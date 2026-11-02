# Banking & Financial Management System

A C++17 console application for managing customers, accounts, transactions, loans, employees, and basic banking reports. Menus use numbered choices and keep operational records in the local `data/` directory.

## Run on Windows

Install a C++17 compiler such as MinGW g++, then run `run.bat` from File Explorer or a Command Prompt. The script compiles the application sources and starts the program. The default administrator login is `admin` with password `123`.

When the program starts, choose administrator login or customer login. The administrator menu provides:

1. Customer and account management
2. Deposits, withdrawals, transfers, and transaction history
3. Loan applications, review, and repayments
4. Employee register and salary maintenance
5. Reports and analytics
6. Administrator password and profile settings
7. Logout

## Customer access

Customer IDs are the serial numbers shown in `data/customer.txt`. An administrator can set a customer's initial password during registration or choose **Customer management → Create or reset customer login** for an existing customer. Give the customer their serial number and the password they set.

After signing in, customers can view their profile, account balances, transaction history for their own accounts, and loan applications. They can also submit a loan application and change their password. Customer accounts do not have access to administrator actions such as transfers, withdrawals, customer records, or reports.

## Local data files

Customer details use the existing formatted table in `data/customer.txt`. Account, transaction, loan, employee, and customer login records are stored under `data/`. Keep the records in place between runs to retain the application data. Administrator and customer passwords are stored as local plain text in `data/admin.txt` and `data/customer_credentials.txt`; both files are ignored by Git.

This is a learning project, not production banking software. Its local text-file records and password storage are not encrypted and do not provide production-grade security, audit controls, concurrency protection, or recovery guarantees. Do not use real customer or financial data.
