#include <fstream>
#include <iomanip>
#include <sstream>
#include "include/account.h"

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
