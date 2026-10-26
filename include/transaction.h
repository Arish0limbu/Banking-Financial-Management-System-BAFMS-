#ifndef BAFMS_TRANSACTION_H
#define BAFMS_TRANSACTION_H

#include <string>
#include <vector>

struct TransactionRecord
{
    int transactionId;
    std::string type;
    int fromAccount;
    int toAccount;
    double amount;
    std::string timestamp;
};

std::vector<TransactionRecord> readTransactions();
bool saveTransaction(const std::string &type, int fromAccount, int toAccount, double amount);
void transactionmanagement();

#endif
