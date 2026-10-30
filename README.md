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

## Local browser demo

The repository also includes a small HTML/CSS/JavaScript dashboard backed by the existing C++ account and transaction functions. On Windows, install Node.js and MinGW g++, then run `run-web-demo.bat` and open [http://127.0.0.1:4173](http://127.0.0.1:4173). Keep the terminal window open while using the page; press Ctrl+C to stop it.

The demo binds to your computer only and uses fictional records in `web-demo/runtime/data/`. Deposits, withdrawals, transfers, and their history update those demo files, not the regular `data/` records. To reset the demo, stop the server, delete `accounts.txt` and `transactions.txt` from `web-demo/runtime/data/`, and start it again. It does not include administrator login, customer management, loans, or employee management yet.

This is a local prototype. It is not configured for public hosting or real banking data.

## Local data files

Customer details use the existing formatted table in `data/customer.txt`. Account, transaction, loan, and employee records are stored as pipe-separated rows in their corresponding files under `data/`. Keep the records in place between runs to retain the application data. The administrator password is saved in `data/admin.txt` after it is changed; that local settings file is ignored by Git.

This is a learning project, not production banking software. Its local text-file records and password storage are not encrypted and do not provide production-grade security, audit controls, concurrency protection, or recovery guarantees. Do not use real customer or financial data.
