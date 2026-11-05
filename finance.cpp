#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "include/account.h"
#include "include/design.h"
#include "include/employee.h"
#include "include/finance.h"
#include "include/loan.h"
#include "include/transaction.h"

using namespace std;

static int countCustomers()
{
    ifstream file("data/customer.txt");
    string row;
    int count = 0;
    set<int> customerSerials;
    while (getline(file, row))
    {
        size_t separator = row.find('|');
        if (separator == string::npos)
            continue;
        try
        {
            string serialText = row.substr(0, separator);
            size_t start = serialText.find_first_not_of(" \t");
            if (start == string::npos)
                continue;
            size_t end = serialText.find_last_not_of(" \t");
            serialText = serialText.substr(start, end - start + 1);
            size_t consumed = 0;
            int serial = stoi(serialText, &consumed);
            if (serial > 0 && consumed == serialText.size() && customerSerials.insert(serial).second)
                ++count;
        }
        catch (...)
        {
            continue;
        }
    }
    return count;
}

void showReports()
{
    heading("|| REPORTS & ANALYTICS ||");
    vector<AccountRecord> accounts = readAccounts();
    vector<LoanRecord> loans = readLoans();
    vector<EmployeeRecord> employees = readEmployees();
    vector<TransactionRecord> transactions = readTransactions();

    int activeAccounts = 0;
    double totalDeposits = 0;
    for (const AccountRecord &account : accounts)
        if (account.active)
        {
            ++activeAccounts;
            totalDeposits += account.balance;
        }

    int pendingLoans = 0;
    int approvedLoans = 0;
    int rejectedLoans = 0;
    int paidLoans = 0;
    double outstandingLoans = 0;
    for (const LoanRecord &loan : loans)
    {
        if (loan.status == "Pending")
            ++pendingLoans;
        if (loan.status == "Approved")
        {
            ++approvedLoans;
            outstandingLoans += loanOutstandingBalance(loan);
        }
        else if (loan.status == "Rejected")
            ++rejectedLoans;
        else if (loan.status == "Paid")
            ++paidLoans;
    }

    int activeEmployees = 0;
    double monthlyPayroll = 0;
    map<string, pair<int, double>> payrollByRole;
    for (const EmployeeRecord &employee : employees)
        if (employee.active)
        {
            ++activeEmployees;
            monthlyPayroll += employee.salary;
            pair<int, double> &roleSummary = payrollByRole[employee.role];
            ++roleSummary.first;
            roleSummary.second += employee.salary;
        }

    struct MonthlyVolumes
    {
        int transactionCount = 0;
        double deposits = 0;
        double withdrawals = 0;
        double loanPayments = 0;
        double transfers = 0;
        double netCashFlow = 0;
    };
    double depositVolume = 0;
    double withdrawalVolume = 0;
    double loanPaymentVolume = 0;
    double transferVolume = 0;
    map<string, MonthlyVolumes> monthlyVolumes;
    for (const TransactionRecord &transaction : transactions)
    {
        MonthlyVolumes *month = nullptr;
        if (transaction.timestamp != "unknown" && transaction.timestamp.size() >= 7)
            month = &monthlyVolumes[transaction.timestamp.substr(0, 7)];
        if (month)
            ++month->transactionCount;
        if (transaction.type == "DEPOSIT")
        {
            depositVolume += transaction.amount;
            if (month)
            {
                month->deposits += transaction.amount;
                month->netCashFlow += transaction.amount;
            }
        }
        else if (transaction.type == "WITHDRAWAL")
        {
            withdrawalVolume += transaction.amount;
            if (month)
            {
                month->withdrawals += transaction.amount;
                month->netCashFlow -= transaction.amount;
            }
        }
        else if (transaction.type == "LOAN_PAYMENT")
        {
            loanPaymentVolume += transaction.amount;
            if (month)
            {
                month->loanPayments += transaction.amount;
                month->netCashFlow -= transaction.amount;
            }
        }
        else if (transaction.type == "TRANSFER")
        {
            transferVolume += transaction.amount;
            if (month)
                month->transfers += transaction.amount;
        }
    }

    cout << fixed << setprecision(2);
    cout << "Customers                    : " << countCustomers() << '\n'
         << "Active accounts              : " << activeAccounts << " / " << accounts.size() << '\n'
         << "Total active account balance : " << totalDeposits << '\n'
         << "Deposit transaction volume   : " << depositVolume << '\n'
         << "Withdrawal transaction volume: " << withdrawalVolume << '\n'
         << "Loan payment volume          : " << loanPaymentVolume << '\n'
         << "Transfer transaction volume  : " << transferVolume << '\n'
         << "Approved loan applications  : " << approvedLoans << '\n'
         << "Approved loan outstanding   : " << outstandingLoans << '\n'
         << "Pending loan applications   : " << pendingLoans << '\n'
         << "Rejected loan applications  : " << rejectedLoans << '\n'
         << "Paid loan applications      : " << paidLoans << '\n'
         << "Transactions recorded        : " << transactions.size() << '\n'
         << "Active employees             : " << activeEmployees << " / " << employees.size() << '\n'
         << "Monthly payroll              : " << monthlyPayroll << '\n';
    line();
    cout << "Loan amounts use simple interest over the selected term.\n";
    cout << "\nMonthly transaction volume (records with timestamps):\n";
    if (monthlyVolumes.empty())
        cout << "No dated transactions are available.\n";
    else
    {
        cout << left << setw(12) << "MONTH" << setw(10) << "COUNT" << setw(14) << "DEPOSITS"
             << setw(14) << "WITHDRAWALS" << setw(16) << "LOAN PAYMENTS"
             << setw(14) << "TRANSFERS" << "NET FLOW\n";
        line();
        for (const auto &entry : monthlyVolumes)
            cout << left << setw(12) << entry.first << setw(10) << entry.second.transactionCount
                 << setw(14) << entry.second.deposits
                 << setw(14) << entry.second.withdrawals << setw(16) << entry.second.loanPayments
                 << setw(14) << entry.second.transfers << entry.second.netCashFlow << '\n';
    }
    ps();
}

void showAccountReconciliation()
{
    heading("|| ACCOUNT BALANCE RECONCILIATION ||");
    const vector<AccountRecord> accounts = readAccounts();
    if (accounts.empty())
    {
        cout << "No account records are available to reconcile.\n";
        ps();
        return;
    }

    map<int, const AccountRecord *> accountByNumber;
    map<int, double> ledgerBalances;
    for (const AccountRecord &account : accounts)
    {
        accountByNumber[account.accountNumber] = &account;
        ledgerBalances[account.accountNumber] = 0.0;
    }

    int missingAccountReferences = 0;
    for (const TransactionRecord &transaction : readTransactions())
    {
        if (transaction.fromAccount > 0)
        {
            auto account = ledgerBalances.find(transaction.fromAccount);
            if (account == ledgerBalances.end())
                ++missingAccountReferences;
            else
                account->second -= transaction.amount;
        }
        if (transaction.toAccount > 0)
        {
            auto account = ledgerBalances.find(transaction.toAccount);
            if (account == ledgerBalances.end())
                ++missingAccountReferences;
            else
                account->second += transaction.amount;
        }
    }

    int matchedAccounts = 0;
    int mismatchedAccounts = 0;
    cout << left << setw(14) << "ACCOUNT" << setw(14) << "CUSTOMER SN"
         << setw(16) << "LEDGER" << setw(16) << "STORED" << "DIFFERENCE\n";
    line();
    cout << fixed << setprecision(2);
    for (const auto &entry : accountByNumber)
    {
        const AccountRecord &account = *entry.second;
        const double ledgerBalance = round(ledgerBalances[entry.first] * 100.0) / 100.0;
        const double difference = round((account.balance - ledgerBalance) * 100.0) / 100.0;
        if (fabs(difference) <= 0.005)
        {
            ++matchedAccounts;
            continue;
        }
        ++mismatchedAccounts;
        cout << left << setw(14) << account.accountNumber << setw(14) << account.customerSerial
             << setw(16) << ledgerBalance << setw(16) << account.balance << difference << '\n';
    }
    if (mismatchedAccounts == 0)
        cout << "All account balances match the recorded transaction ledger.\n";
    cout << "Accounts checked: " << accounts.size() << " | Matching: " << matchedAccounts
         << " | Mismatched: " << mismatchedAccounts << '\n'
         << "Transaction references to missing accounts: " << missingAccountReferences << '\n';
    cout << "This comparison assumes each account started with a zero balance.\n";
    ps();
}
