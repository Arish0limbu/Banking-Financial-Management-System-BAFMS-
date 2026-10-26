#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "include/design.h"
#include "include/loan.h"

using namespace std;

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
            !getline(fields, status, '|'))
            continue;
        try
        {
            LoanRecord loan{stoi(id), stoi(customer), stod(principal), stod(rate),
                            stoi(months), stod(paid), status};
            if (loan.loanId > 0 && loan.customerSerial > 0 && loan.principal > 0 &&
                loan.annualRate >= 0 && loan.termMonths > 0 && loan.amountPaid >= 0)
                loans.push_back(loan);
        }
        catch (...)
        {
            continue;
        }
    }
    return loans;
}

bool saveLoans(const vector<LoanRecord> &loans)
{
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
            if (stoi(row.substr(0, separator)) == serial)
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
    return loan.principal * (1.0 + loan.annualRate * loan.termMonths / 1200.0);
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
    if (!readdouble("Requested principal: ", principal, 1.0, 10000000.0) ||
        !readint("Term in months (1-360): ", term, 1, 360))
        return;

    vector<LoanRecord> loans = readLoans();
    int nextId = 1;
    for (const LoanRecord &loan : loans)
        if (loan.loanId >= nextId)
            nextId = loan.loanId + 1;
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

void loanmanagement()
{
    int choice;
    do
    {
        heading("|| LOAN MANAGEMENT ||");
        cout << "1. Apply for loan\n2. View loan register\n3. Review pending application\n4. Back\n";
        if (!readint("Select an option: ", choice, 1, 4))
            return;
        if (choice == 1)
            applyForLoan();
        else if (choice == 2)
            listLoans();
        else if (choice == 3)
            reviewApplications();
    } while (choice != 4);
}
