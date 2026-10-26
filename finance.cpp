#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
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
    while (getline(file, row))
    {
        size_t separator = row.find('|');
        if (separator == string::npos)
            continue;
        try
        {
            if (stoi(row.substr(0, separator)) > 0)
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
    double outstandingLoans = 0;
    for (const LoanRecord &loan : loans)
    {
        if (loan.status == "Pending")
            ++pendingLoans;
        if (loan.status == "Approved")
            outstandingLoans += max(0.0, loan.principal * (1.0 + loan.annualRate * loan.termMonths / 1200.0) - loan.amountPaid);
    }

    int activeEmployees = 0;
    double monthlyPayroll = 0;
    for (const EmployeeRecord &employee : employees)
        if (employee.active)
        {
            ++activeEmployees;
            monthlyPayroll += employee.salary;
        }

    double depositVolume = 0;
    double withdrawalVolume = 0;
    for (const TransactionRecord &transaction : transactions)
    {
        if (transaction.type == "DEPOSIT")
            depositVolume += transaction.amount;
        else if (transaction.type == "WITHDRAWAL" || transaction.type == "LOAN_PAYMENT")
            withdrawalVolume += transaction.amount;
    }

    cout << fixed << setprecision(2);
    cout << "Customers                    : " << countCustomers() << '\n'
         << "Active accounts              : " << activeAccounts << " / " << accounts.size() << '\n'
         << "Total active account balance : " << totalDeposits << '\n'
         << "Deposit transaction volume   : " << depositVolume << '\n'
         << "Withdrawal / loan payments   : " << withdrawalVolume << '\n'
         << "Approved loan outstanding   : " << outstandingLoans << '\n'
         << "Pending loan applications   : " << pendingLoans << '\n'
         << "Transactions recorded        : " << transactions.size() << '\n'
         << "Active employees             : " << activeEmployees << " / " << employees.size() << '\n'
         << "Monthly payroll              : " << monthlyPayroll << '\n';
    line();
    cout << "Loan amounts use simple interest over the selected term.\n";
    ps();
}
