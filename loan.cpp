#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include "include/account.h"
#include "include/design.h"
#include "include/loan.h"
#include "include/transaction.h"

using namespace std;

static double totalDue(const LoanRecord &loan);

static bool parseLoanInteger(const string &text, int &value)
{
    try
    {
        size_t consumed = 0;
        value = stoi(text, &consumed);
        return consumed == text.size();
    }
    catch (...)
    {
        return false;
    }
}

static bool parseLoanAmount(const string &text, double &value)
{
    try
    {
        size_t consumed = 0;
        value = stod(text, &consumed);
        return consumed == text.size() && isfinite(value);
    }
    catch (...)
    {
        return false;
    }
}

static bool hasCentPrecision(double value)
{
    return isfinite(value) && fabs(value * 100.0 - round(value * 100.0)) < 1e-7;
}

static bool validLoanRecord(const LoanRecord &loan)
{
    const bool knownStatus = loan.status == "Pending" || loan.status == "Approved" ||
                             loan.status == "Rejected" || loan.status == "Paid";
    const double due = totalDue(loan);
    return loan.loanId > 0 && loan.customerSerial > 0 && hasCentPrecision(loan.principal) &&
           loan.principal > 0 && isfinite(loan.annualRate) && loan.annualRate >= 0 &&
           loan.termMonths > 0 && hasCentPrecision(loan.amountPaid) && loan.amountPaid >= 0 &&
           isfinite(due) && loan.amountPaid <= due + 0.005 && knownStatus;
}

vector<LoanRecord> readLoans()
{
    vector<LoanRecord> loans;
    ifstream file("data/loans.txt");
    string row;
    while (getline(file, row))
    {
        stringstream fields(row);
        string id, customer, principal, rate, months, paid, status;
        if (!getline(fields, id, '|') || !getline(fields, customer, '|') ||
            !getline(fields, principal, '|') || !getline(fields, rate, '|') ||
            !getline(fields, months, '|') || !getline(fields, paid, '|') ||
            !getline(fields, status))
            continue;

        LoanRecord loan{};
        if (!parseLoanInteger(id, loan.loanId) ||
            !parseLoanInteger(customer, loan.customerSerial) ||
            !parseLoanAmount(principal, loan.principal) ||
            !parseLoanAmount(rate, loan.annualRate) ||
            !parseLoanInteger(months, loan.termMonths) ||
            !parseLoanAmount(paid, loan.amountPaid))
            continue;
        loan.status = status;
        const bool duplicateId = any_of(loans.begin(), loans.end(),
                                        [&loan](const LoanRecord &stored)
                                        { return stored.loanId == loan.loanId; });
        if (validLoanRecord(loan) && !duplicateId)
            loans.push_back(loan);
    }
    return loans;
}

bool saveLoans(const vector<LoanRecord> &loans)
{
    for (size_t i = 0; i < loans.size(); ++i)
    {
        if (!validLoanRecord(loans[i]))
            return false;
        for (size_t j = 0; j < i; ++j)
            if (loans[j].loanId == loans[i].loanId)
                return false;
    }

    ofstream file("data/loans.txt", ios::trunc);
    if (!file.is_open())
        return false;
    file << fixed << setprecision(2);
    for (const LoanRecord &loan : loans)
        file << loan.loanId << '|' << loan.customerSerial << '|' << loan.principal << '|'
             << loan.annualRate << '|' << loan.termMonths << '|' << loan.amountPaid << '|'
             << loan.status << '\n';
    return file.good();
}

static bool loanCustomerExists(int serial)
{
    ifstream file("data/customer.txt");
    string row;
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
            int storedSerial = stoi(serialText, &consumed);
            if (consumed == serialText.size() && storedSerial > 0 && storedSerial == serial)
                return true;
        }
        catch (...)
        {
            continue;
        }
    }
    return false;
}

static double totalDue(const LoanRecord &loan)
{
    const double amount = loan.principal * (1.0 + loan.annualRate * loan.termMonths / 1200.0);
    return round(amount * 100.0) / 100.0;
}

static void applyForLoan()
{
    int serial, term;
    double principal;
    heading("|| LOAN APPLICATION ||");
    if (!readint("Customer serial number: ", serial, 1, 1000000))
        return;
    if (!loanCustomerExists(serial))
    {
        cout << "Customer serial number was not found.\n";
        ps();
        return;
    }
    if (!readmoney("Requested principal: ", principal, 1.0, 10000000.0) ||
        !readint("Term in months (1-360): ", term, 1, 360))
        return;

    vector<LoanRecord> loans = readLoans();
    int nextId = 1;
    for (const LoanRecord &loan : loans)
        if (loan.loanId >= nextId)
        {
            if (loan.loanId == numeric_limits<int>::max())
            {
                cout << "No more loan application IDs are available.\n";
                ps();
                return;
            }
            nextId = loan.loanId + 1;
        }
    loans.push_back({nextId, serial, principal, 10.0, term, 0.0, "Pending"});
    if (!saveLoans(loans))
        cout << "Unable to save the loan application.\n";
    else
        cout << "Application " << nextId << " submitted at 10% annual simple interest.\n";
    ps();
}

static void listLoans()
{
    heading("|| LOAN REGISTER ||");
    vector<LoanRecord> loans = readLoans();
    if (loans.empty())
        cout << "No loan applications have been recorded.\n";
    else
    {
        cout << left << setw(8) << "ID" << setw(12) << "CUSTOMER" << setw(14) << "PRINCIPAL"
             << setw(12) << "RATE" << setw(10) << "MONTHS" << setw(14) << "OUTSTANDING" << "STATUS\n";
        line();
        cout << fixed << setprecision(2);
        for (const LoanRecord &loan : loans)
            cout << left << setw(8) << loan.loanId << setw(12) << loan.customerSerial
                 << setw(14) << loan.principal << setw(12) << loan.annualRate
                 << setw(10) << loan.termMonths << setw(14) << max(0.0, totalDue(loan) - loan.amountPaid)
                 << loan.status << '\n';
    }
    ps();
}

static void reviewApplications()
{
    vector<LoanRecord> loans = readLoans();
    int id;
    if (!readint("Pending application ID: ", id, 1, 2147483647))
        return;
    auto found = loans.end();
    for (auto it = loans.begin(); it != loans.end(); ++it)
        if (it->loanId == id && it->status == "Pending")
        {
            found = it;
            break;
        }
    if (found == loans.end())
    {
        cout << "Pending application was not found.\n";
        ps();
        return;
    }

    int decision;
    cout << "1. Approve\n2. Reject\n";
    if (!readint("Decision: ", decision, 1, 2))
        return;
    found->status = decision == 1 ? "Approved" : "Rejected";
    if (!saveLoans(loans))
        cout << "Unable to save the loan decision.\n";
    else
        cout << "Application " << id << " " << found->status << ".\n";
    ps();
}

static void makeRepayment()
{
    int loanId, accountNumber;
    double amount;
    vector<LoanRecord> loans = readLoans();
    if (!readint("Approved loan ID: ", loanId, 1, 2147483647) ||
        !readint("Payment account number: ", accountNumber, 1, 2147483647))
        return;

    auto found = loans.end();
    for (auto it = loans.begin(); it != loans.end(); ++it)
        if (it->loanId == loanId && it->status == "Approved")
        {
            found = it;
            break;
        }
    AccountRecord account;
    if (found == loans.end() || !findAccount(accountNumber, account) || !account.active ||
        account.customerSerial != found->customerSerial)
    {
        cout << "Use an approved loan and an active account belonging to its customer.\n";
        ps();
        return;
    }

    double outstanding = max(0.0, totalDue(*found) - found->amountPaid);
    if (!readmoney("Payment amount: ", amount, 0.01, outstanding))
        return;
    if (amount > account.balance)
    {
        cout << "Insufficient funds in the payment account.\n";
        ps();
        return;
    }

    vector<LoanRecord> originalLoans = loans;
    found->amountPaid += amount;
    if (totalDue(*found) - found->amountPaid <= 0.005)
        found->status = "Paid";

    if (!updateAccountBalance(accountNumber, account.balance - amount))
    {
        cout << "Unable to debit the payment account.\n";
        ps();
        return;
    }
    if (!saveLoans(loans))
    {
        if (updateAccountBalance(accountNumber, account.balance))
            cout << "Unable to update the loan record; the account balance was restored.\n";
        else
            cout << "Loan update failed and the account balance could not be restored. Check both records.\n";
        ps();
        return;
    }
    if (!saveTransaction("LOAN_PAYMENT", accountNumber, 0, amount))
    {
        const bool loanRestored = saveLoans(originalLoans);
        const bool accountRestored = updateAccountBalance(accountNumber, account.balance);
        if (loanRestored && accountRestored)
            cout << "Payment was not recorded; the previous balances were restored.\n";
        else
            cout << "Payment logging failed and rollback was incomplete. Check the loan and account records.\n";
        ps();
        return;
    }
    cout << "Payment recorded. Remaining loan balance: "
         << fixed << setprecision(2) << max(0.0, totalDue(*found) - found->amountPaid) << '\n';
    ps();
}

void loanmanagement()
{
    int choice;
    do
    {
        heading("|| LOAN MANAGEMENT ||");
        cout << "1. Apply for loan\n2. View loan register\n3. Review pending application\n"
             << "4. Make loan repayment\n5. Back\n";
        if (!readint("Select an option: ", choice, 1, 5))
            return;
        if (choice == 1)
            applyForLoan();
        else if (choice == 2)
            listLoans();
        else if (choice == 3)
            reviewApplications();
        else if (choice == 4)
            makeRepayment();
    } while (choice != 5);
}
