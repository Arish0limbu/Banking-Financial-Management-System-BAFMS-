#ifndef BAFMS_ACCOUNT_H
#define BAFMS_ACCOUNT_H

#include <string>
#include <vector>

struct AccountRecord
{
    int accountNumber;
    int customerSerial;
    std::string accountType;
    double balance;
    bool active;
};

std::vector<AccountRecord> readAccounts();
bool saveAccounts(const std::vector<AccountRecord> &accounts);
bool findAccount(int accountNumber, AccountRecord &account);
bool updateAccountBalance(int accountNumber, double newBalance);
void accountmanagement();

#endif
