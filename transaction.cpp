#include <ctime>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "include/account.h"
#include "include/design.h"
#include "include/transaction.h"

using namespace std;

static bool parseTransactionInteger(const string &text, int &value)
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

static bool parseTransactionAmount(const string &text, double &value)
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

static bool validTransaction(const TransactionRecord &transaction)
{
    if (transaction.transactionId <= 0 || !isfinite(transaction.amount) || transaction.amount <= 0)
        return false;

    if (transaction.type == "DEPOSIT")
        return transaction.fromAccount == 0 && transaction.toAccount > 0;
    if (transaction.type == "WITHDRAWAL" || transaction.type == "LOAN_PAYMENT")
        return transaction.fromAccount > 0 && transaction.toAccount == 0;
    if (transaction.type == "TRANSFER")
        return transaction.fromAccount > 0 && transaction.toAccount > 0 &&
               transaction.fromAccount != transaction.toAccount;
    return false;
}

vector<TransactionRecord> readTransactions()
{
    vector<TransactionRecord> transactions;
    ifstream file("data/transactions.txt");
    string row;

    while (getline(file, row))
    {
        stringstream fields(row);
        string id, type, from, to, amount, timestamp;
        if (!getline(fields, id, '|') || !getline(fields, type, '|') ||
            !getline(fields, from, '|') || !getline(fields, to, '|') ||
            !getline(fields, amount, '|') || !getline(fields, timestamp))
            continue;
        TransactionRecord transaction{};
        if (!parseTransactionInteger(id, transaction.transactionId) ||
            !parseTransactionInteger(from, transaction.fromAccount) ||
            !parseTransactionInteger(to, transaction.toAccount) ||
            !parseTransactionAmount(amount, transaction.amount))
            continue;
        transaction.type = type;
        transaction.timestamp = timestamp;
        if (validTransaction(transaction))
            transactions.push_back(transaction);
    }
    return transactions;
}

bool saveTransaction(const string &type, int fromAccount, int toAccount, double amount)
{
    TransactionRecord newTransaction{1, type, fromAccount, toAccount, amount, ""};
    if (!validTransaction(newTransaction))
        return false;

    int nextId = 1;
    for (const TransactionRecord &transaction : readTransactions())
        if (transaction.transactionId >= nextId)
            nextId = transaction.transactionId + 1;

    time_t now = time(nullptr);
    tm *local = localtime(&now);
    char timestamp[32] = "unknown";
    if (local)
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", local);

    ofstream file("data/transactions.txt", ios::app);
    if (!file.is_open())
        return false;
    file << nextId << '|' << type << '|' << fromAccount << '|' << toAccount << '|'
         << fixed << setprecision(2) << amount << '|' << timestamp << '\n';
    return file.good();
}

static void showHistory()
{
    heading("|| TRANSACTION HISTORY ||");
    vector<TransactionRecord> transactions = readTransactions();
    if (transactions.empty())
        cout << "No transactions have been recorded.\n";
    else
    {
        cout << left << setw(8) << "ID" << setw(18) << "TYPE" << setw(14) << "FROM"
             << setw(14) << "TO" << setw(14) << "AMOUNT" << "DATE / TIME\n";
        line();
        cout << fixed << setprecision(2);
        for (auto it = transactions.rbegin(); it != transactions.rend(); ++it)
            cout << left << setw(8) << it->transactionId << setw(18) << it->type
                 << setw(14) << (it->fromAccount == 0 ? "-" : to_string(it->fromAccount))
                 << setw(14) << (it->toAccount == 0 ? "-" : to_string(it->toAccount))
                 << setw(14) << it->amount << it->timestamp << '\n';
    }
    ps();
}

static void depositFunds()
{
    int number;
    double amount;
    if (!readint("Account number: ", number, 1, 2147483647))
        return;

    AccountRecord account;
    if (!findAccount(number, account) || !account.active)
    {
        cout << "Active account was not found.\n";
        ps();
        return;
    }
    if (!readmoney("Deposit amount: ", amount, 0.01, 1000000000.0))
        return;

    if (!updateAccountBalance(number, account.balance + amount))
    {
        cout << "Unable to update the account balance.\n";
        ps();
        return;
    }
    if (!saveTransaction("DEPOSIT", 0, number, amount))
    {
        updateAccountBalance(number, account.balance);
        cout << "Deposit was not recorded; the previous balance was restored.\n";
        ps();
        return;
    }
    cout << fixed << setprecision(2) << "Deposit successful. New balance: " << account.balance + amount << '\n';
    ps();
}

static void withdrawFunds()
{
    int number;
    double amount;
    if (!readint("Account number: ", number, 1, 2147483647))
        return;

    AccountRecord account;
    if (!findAccount(number, account) || !account.active)
    {
        cout << "Active account was not found.\n";
        ps();
        return;
    }
    if (!readmoney("Withdrawal amount: ", amount, 0.01, 1000000000.0))
        return;
    if (amount > account.balance)
    {
        cout << fixed << setprecision(2) << "Insufficient funds. Available balance: " << account.balance << '\n';
        ps();
        return;
    }

    if (!updateAccountBalance(number, account.balance - amount))
    {
        cout << "Unable to update the account balance.\n";
        ps();
        return;
    }
    if (!saveTransaction("WITHDRAWAL", number, 0, amount))
    {
        updateAccountBalance(number, account.balance);
        cout << "Withdrawal was not recorded; the previous balance was restored.\n";
        ps();
        return;
    }
    cout << fixed << setprecision(2) << "Withdrawal successful. New balance: " << account.balance - amount << '\n';
    ps();
}

static void transferFunds()
{
    int sourceNumber, destinationNumber;
    double amount;
    if (!readint("Source account number: ", sourceNumber, 1, 2147483647) ||
        !readint("Destination account number: ", destinationNumber, 1, 2147483647))
        return;
    if (sourceNumber == destinationNumber)
    {
        cout << "Choose two different accounts.\n";
        ps();
        return;
    }
    if (!readmoney("Transfer amount: ", amount, 0.01, 1000000000.0))
        return;

    vector<AccountRecord> accounts = readAccounts();
    auto source = accounts.end();
    auto destination = accounts.end();
    for (auto it = accounts.begin(); it != accounts.end(); ++it)
    {
        if (it->accountNumber == sourceNumber && it->active)
            source = it;
        if (it->accountNumber == destinationNumber && it->active)
            destination = it;
    }
    if (source == accounts.end() || destination == accounts.end())
    {
        cout << "Both accounts must exist and be active.\n";
        ps();
        return;
    }
    if (source->balance < amount)
    {
        cout << "Insufficient funds in the source account.\n";
        ps();
        return;
    }

    const double sourceBalance = source->balance;
    const double destinationBalance = destination->balance;
    source->balance -= amount;
    destination->balance += amount;
    if (!saveAccounts(accounts))
    {
        cout << "Unable to update account balances.\n";
        ps();
        return;
    }
    if (!saveTransaction("TRANSFER", sourceNumber, destinationNumber, amount))
    {
        source->balance = sourceBalance;
        destination->balance = destinationBalance;
        saveAccounts(accounts);
        cout << "Transfer was not recorded; the previous balances were restored.\n";
        ps();
        return;
    }
    cout << "Transfer completed successfully.\n";
    ps();
}

void transactionmanagement()
{
    int choice;
    do
    {
        heading("|| TRANSACTION MANAGEMENT ||");
        cout << "1. Deposit funds\n2. Withdraw funds\n3. Transfer funds\n"
             << "4. View transaction history\n5. Back\n";
        if (!readint("Select an option: ", choice, 1, 5))
            return;
        if (choice == 1)
            depositFunds();
        else if (choice == 2)
            withdrawFunds();
        else if (choice == 3)
            transferFunds();
        else if (choice == 4)
            showHistory();
    } while (choice != 5);
}
