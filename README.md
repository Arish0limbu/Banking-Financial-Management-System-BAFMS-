# Banking & Financial Management System

A C++17 console application for managing customers, accounts, transactions, loans, employees, and basic banking reports. Menus use numbered choices and keep operational records in the local `data/` directory.

## Run on Windows

Install a C++17 compiler such as MinGW g++, then run `run.bat` from File Explorer or a Command Prompt. The script compiles the application sources and starts the program. The default administrator login is `admin` with password `123`.

After signing in, the main menu provides:

1. Customer and account management
2. Deposits, withdrawals, transfers, and transaction history
3. Loan applications, review, and repayments
4. Employee register and salary maintenance
5. Reports and analytics
6. Administrator password and profile settings
7. Logout

## Local data files

Customer details use the existing formatted table in `data/customer.txt`. Account, transaction, loan, and employee records are stored as pipe-separated rows in their corresponding files under `data/`. Keep the records in place between runs to retain the application data. The administrator password is saved in `data/admin.txt` after it is changed; that local settings file is ignored by Git.

This is a learning project, not production banking software. Its local text-file records and password storage are not encrypted and do not provide production-grade security, audit controls, concurrency protection, or recovery guarantees. Do not use real customer or financial data.
