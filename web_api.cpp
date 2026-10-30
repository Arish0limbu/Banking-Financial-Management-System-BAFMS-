#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "include/account.h"
#include "include/transaction.h"

using namespace std;

static void printJsonString(const string &value)
{
    cout << '"';
    for (unsigned char ch : value)
    {
        switch (ch)
        {
        case '"': cout << "\\\""; break;
        case '\\': cout << "\\\\"; break;
        case '\n': cout << "\\n"; break;
        case '\r': cout << "\\r"; break;
        case '\t': cout << "\\t"; break;
        default:
            if (ch < 0x20)
                cout << ' ';
            else
                cout << static_cast<char>(ch);
        }
    }
    cout << '"';
}

static int fail(const string &message)
{
    cout << "{\"error\":";
    printJsonString(message);
    cout << "}\n";
    return 1;
}

static bool parsePositiveInt(const char *text, int &value)
{
    try
    {
        size_t consumed = 0;
        value = stoi(text, &consumed);
        return consumed == string(text).size() && value > 0;
    }
    catch (...)
    {
        return false;
    }
}

static bool parseAmountCents(const char *text, long long &cents)
{
    try
    {
        size_t consumed = 0;
        cents = stoll(text, &consumed);
        return consumed == string(text).size() && cents >= 1 && cents <= 100000000000LL;
    }
    catch (...)
    {
        return false;
    }
}

static int listAccounts()
{
    const vector<AccountRecord> accounts = readAccounts();
    cout << "{\"accounts\":[";
    for (size_t i = 0; i < accounts.size(); ++i)
    {
        const AccountRecord &account = accounts[i];
        if (i)
            cout << ',';
        cout << "{\"accountNumber\":" << account.accountNumber
             << ",\"customerSerial\":" << account.customerSerial << ",\"type\":";
        printJsonString(account.accountType);
        cout << ",\"balance\":" << fixed << setprecision(2) << account.balance
             << ",\"active\":" << (account.active ? "true" : "false") << '}';
    }
    cout << "]}\n";
    return 0;
}

static int listTransactions()
{
    const vector<TransactionRecord> transactions = readTransactions();
    cout << "{\"transactions\":[";
    for (size_t i = 0; i < transactions.size(); ++i)
    {
        const TransactionRecord &transaction = transactions[i];
        if (i)
            cout << ',';
        cout << "{\"transactionId\":" << transaction.transactionId << ",\"type\":";
        printJsonString(transaction.type);
        cout << ",\"fromAccount\":" << transaction.fromAccount
             << ",\"toAccount\":" << transaction.toAccount
             << ",\"amount\":" << fixed << setprecision(2) << transaction.amount
             << ",\"timestamp\":";
        printJsonString(transaction.timestamp);
        cout << '}';
    }
    cout << "]}\n";
    return 0;
}

static int recordTransaction(int argc, char **argv)
{
    if (argc != 6)
        return fail("Invalid transaction request.");

    const string type = argv[2];
    int accountNumber = 0;
    int destinationNumber = 0;
    long long cents = 0;
    if ((type != "DEPOSIT" && type != "WITHDRAWAL" && type != "TRANSFER") ||
        !parsePositiveInt(argv[3], accountNumber) ||
        !parseAmountCents(argv[5], cents))
        return fail("Enter a valid transaction and positive amount.");

    if (type == "TRANSFER")
    {
        if (!parsePositiveInt(argv[4], destinationNumber) || accountNumber == destinationNumber)
            return fail("Choose two different active accounts.");
    }
    else if (string(argv[4]) != "0")
        return fail("Invalid transaction destination.");

    const double amount = static_cast<double>(cents) / 100.0;
    if (type == "TRANSFER")
    {
        vector<AccountRecord> accounts = readAccounts();
        auto source = accounts.end();
        auto destination = accounts.end();
        for (auto it = accounts.begin(); it != accounts.end(); ++it)
        {
            if (it->accountNumber == accountNumber && it->active)
                source = it;
            if (it->accountNumber == destinationNumber && it->active)
                destination = it;
        }
        if (source == accounts.end() || destination == accounts.end())
            return fail("Both accounts must exist and be active.");
        if (source->balance < amount)
            return fail("The source account has insufficient funds.");

        const double sourceBalance = source->balance;
        const double destinationBalance = destination->balance;
        source->balance -= amount;
        destination->balance += amount;
        if (!isfinite(destination->balance) || !saveAccounts(accounts))
            return fail("Unable to update account balances.");
        if (!saveTransaction(type, accountNumber, destinationNumber, amount))
        {
            source->balance = sourceBalance;
            destination->balance = destinationBalance;
            saveAccounts(accounts);
            return fail("Transfer logging failed; the previous balances were restored when possible.");
        }
        cout << "{\"success\":true,\"message\":\"Transfer completed.\"}\n";
        return 0;
    }

    AccountRecord account{};
    if (!findAccount(accountNumber, account) || !account.active)
        return fail("Active account was not found.");
    if (type == "WITHDRAWAL" && amount > account.balance)
        return fail("The account has insufficient funds.");

    const double newBalance = type == "DEPOSIT" ? account.balance + amount : account.balance - amount;
    if (!isfinite(newBalance) || !updateAccountBalance(accountNumber, newBalance))
        return fail("Unable to update the account balance.");

    const int fromAccount = type == "WITHDRAWAL" ? accountNumber : 0;
    const int toAccount = type == "DEPOSIT" ? accountNumber : 0;
    if (!saveTransaction(type, fromAccount, toAccount, amount))
    {
        const bool restored = updateAccountBalance(accountNumber, account.balance);
        return fail(restored ? "Transaction logging failed; the balance was restored."
                             : "Transaction logging failed and the balance could not be restored.");
    }

    cout << "{\"success\":true,\"message\":\"Transaction completed.\",\"balance\":"
         << fixed << setprecision(2) << newBalance << "}\n";
    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 2 && string(argv[1]) == "accounts")
        return listAccounts();
    if (argc == 2 && string(argv[1]) == "transactions")
        return listTransactions();
    if (argc >= 2 && string(argv[1]) == "transaction")
        return recordTransaction(argc, argv);
    return fail("Unknown API command.");
}
