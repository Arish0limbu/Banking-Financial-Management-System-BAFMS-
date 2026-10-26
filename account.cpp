#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include "include/account.h"
#include "include/design.h"

using namespace std;

vector<AccountRecord> readAccounts()
{
    vector<AccountRecord> accounts;
    ifstream file("data/accounts.txt");
    string row;

    while (getline(file, row))
    {
        stringstream fields(row);
        string number, customer, type, balance, active;
        if (!getline(fields, number, '|') || !getline(fields, customer, '|') ||
            !getline(fields, type, '|') || !getline(fields, balance, '|') ||
            !getline(fields, active, '|'))
            continue;

        try
        {
            AccountRecord account;
            account.accountNumber = stoi(number);
            account.customerSerial = stoi(customer);
            account.accountType = type;
            account.balance = stod(balance);
            account.active = (active == "1");
            if (account.accountNumber > 0 && account.customerSerial > 0 &&
                account.balance >= 0 && (account.accountType == "Savings" || account.accountType == "Current"))
                accounts.push_back(account);
        }
        catch (...)
        {
            continue;
        }
    }

    return accounts;
}

bool saveAccounts(const vector<AccountRecord> &accounts)
{
    ofstream file("data/accounts.txt", ios::trunc);
    if (!file.is_open())
        return false;

    file << fixed << setprecision(2);
    for (const AccountRecord &account : accounts)
        file << account.accountNumber << '|' << account.customerSerial << '|'
             << account.accountType << '|' << account.balance << '|'
             << (account.active ? 1 : 0) << '\n';

    return file.good();
}

bool findAccount(int accountNumber, AccountRecord &account)
{
    for (const AccountRecord &stored : readAccounts())
    {
        if (stored.accountNumber == accountNumber)
        {
            account = stored;
            return true;
        }
    }
    return false;
}

bool updateAccountBalance(int accountNumber, double newBalance)
{
    if (newBalance < 0)
        return false;

    vector<AccountRecord> accounts = readAccounts();
    for (AccountRecord &account : accounts)
    {
        if (account.accountNumber == accountNumber && account.active)
        {
            account.balance = newBalance;
            return saveAccounts(accounts);
        }
    }
    return false;
}

static bool customerSerialExists(int serial)
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

static void openAccount()
{
    int customerSerial, typeChoice;
    heading("|| OPEN BANK ACCOUNT ||");
    if (!readint("Customer serial number: ", customerSerial, 1, 1000000))
        return;
    if (!customerSerialExists(customerSerial))
    {
        cout << "Customer serial number was not found.\n";
        ps();
        return;
    }

    if (!readint("Account type (1 Savings, 2 Current): ", typeChoice, 1, 2))
        return;

    vector<AccountRecord> accounts = readAccounts();
    int nextNumber = 100001;
    for (const AccountRecord &account : accounts)
        if (account.accountNumber >= nextNumber)
            nextNumber = account.accountNumber + 1;

    AccountRecord account{nextNumber, customerSerial,
                          typeChoice == 1 ? "Savings" : "Current", 0.0, true};
    accounts.push_back(account);
    if (!saveAccounts(accounts))
    {
        cout << "Unable to save account. Check the data folder.\n";
        ps();
        return;
    }

    cout << "Account opened successfully. Account number: " << account.accountNumber << '\n';
    ps();
}

static void listAccounts()
{
    heading("|| BANK ACCOUNTS ||");
    vector<AccountRecord> accounts = readAccounts();
    if (accounts.empty())
        cout << "No accounts have been opened yet.\n";
    else
    {
        cout << left << setw(14) << "ACCOUNT" << setw(14) << "CUSTOMER SN"
             << setw(14) << "TYPE" << setw(14) << "BALANCE" << "STATUS\n";
        line();
        cout << fixed << setprecision(2);
        for (const AccountRecord &account : accounts)
            cout << left << setw(14) << account.accountNumber << setw(14) << account.customerSerial
                 << setw(14) << account.accountType << setw(14) << account.balance
                 << (account.active ? "Active" : "Closed") << '\n';
    }
    ps();
}

static void searchAccount()
{
    int number;
    if (!readint("Account number: ", number, 1, 2147483647))
        return;
    AccountRecord account;
    if (!findAccount(number, account))
    {
        cout << "Account was not found.\n";
        ps();
        return;
    }

    cout << "Account number : " << account.accountNumber << '\n'
         << "Customer SN    : " << account.customerSerial << '\n'
         << "Type           : " << account.accountType << '\n'
         << fixed << setprecision(2) << "Balance        : " << account.balance << '\n'
         << "Status         : " << (account.active ? "Active" : "Closed") << '\n';
    ps();
}

static void closeAccount()
{
    int number;
    if (!readint("Account number to close: ", number, 1, 2147483647))
        return;

    vector<AccountRecord> accounts = readAccounts();
    auto found = accounts.end();
    for (auto it = accounts.begin(); it != accounts.end(); ++it)
        if (it->accountNumber == number && it->active)
        {
            found = it;
            break;
        }

    if (found == accounts.end())
    {
        cout << "Active account was not found.\n";
        ps();
        return;
    }
    if (found->balance > 0.005)
    {
        cout << "An account must have a zero balance before it can be closed.\n";
        ps();
        return;
    }

    char confirm;
    cout << "Close account " << number << "? (Y/N): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "Account closure cancelled.\n";
        ps();
        return;
    }

    found->active = false;
    if (!saveAccounts(accounts))
        cout << "Unable to save account changes.\n";
    else
        cout << "Account closed successfully.\n";
    ps();
}

void accountmanagement()
{
    int choice;
    do
    {
        heading("|| ACCOUNT MANAGEMENT ||");
        cout << "1. Open account\n2. View all accounts\n3. Search account\n"
             << "4. Close account\n5. Back\n";
        if (!readint("Select an option: ", choice, 1, 5))
            return;
        if (choice == 1)
            openAccount();
        else if (choice == 2)
            listAccounts();
        else if (choice == 3)
            searchAccount();
        else if (choice == 4)
            closeAccount();
    } while (choice != 5);
}
