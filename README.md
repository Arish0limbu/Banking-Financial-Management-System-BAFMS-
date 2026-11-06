# Banking & Financial Management System

A C++17 console application for managing customers, accounts, transactions, loans, employees, and basic banking reports. Menus use numbered choices and keep operational records in the local `data/` directory.

## Run on Windows

Install a C++17 compiler such as MinGW g++, then run `run.bat` from File Explorer or a Command Prompt. The script compiles the application sources and starts the program. At the login page, enter one ID and password; the system detects whether the matching account is an administrator or customer. Type `exit` at the ID prompt to close the program. The default administrator login is ID `admin` and password `123`.

After administrator sign-in, the administrator menu provides:

1. Customer and account management
2. Deposits, withdrawals, transfers, and transaction history
3. Loan applications, review, and repayments
4. Employee register and salary maintenance
5. Reports and analytics
6. Administrator password and profile settings
7. Account balance reconciliation against the transaction ledger
8. Export monthly analytics as CSV
9. Logout

Reconciliation is read-only and assumes account balances started at zero. It lists stored balance differences and transaction references to missing accounts.

Administrators can inspect one account's transaction history and filter it by transaction type. Customers can view history for all their accounts or select one account. Exported statement files are ignored by Git to keep local financial records out of commits.

The analytics report also groups transaction counts and deposit, withdrawal, loan payment, and transfer volumes by month for transactions with valid timestamps. Monthly net flow adds deposits and subtracts withdrawals and loan payments; transfers do not change total cash held by the system.

Administrators can export those monthly metrics to `data/monthly_analytics.csv`. The generated report stays local and is ignored by Git.

Active employee counts and monthly payroll totals are also grouped by job role in the analytics report.

## Customer access

Customer IDs are the serial numbers shown in `data/customer.txt`. An administrator can set a customer's initial password during registration or choose **Customer management → Create or reset customer login** for an existing customer. Give the customer their serial number and the password they set. On the shared login page, administrator credentials are checked first; otherwise, the ID and password are checked against configured customer logins. A customer whose login has not been set up must ask the administrator to create it.

After signing in, customers can view their profile, account balances, transaction history for their own accounts, and loan applications. History can be filtered by owned account, transaction type, and month (`YYYY-MM`). Customers can estimate a loan payment using the current 10% annual simple-interest rate, submit an application, change their password, and export an account statement from the customer menu. The estimate shows the principal and term and does not submit an application. Statements are saved as `data/customer_statement_<customer-id>.csv` and include only transactions involving that customer's accounts, with account type and a balance reconstructed from the transaction ledger. Reconstructed balances assume a zero opening balance. Customer accounts do not have access to administrator actions such as transfers, withdrawals, customer records, or reports.

## Local data files

Customer details use the existing formatted table in `data/customer.txt`. Account, transaction, loan, employee, and customer login records are stored under `data/`. Keep the records in place between runs to retain the application data. Administrator and customer passwords are stored as local plain text in `data/admin.txt` and `data/customer_credentials.txt`; both files are ignored by Git.

This is a learning project, not production banking software. Its local text-file records and password storage are not encrypted and do not provide production-grade security, audit controls, concurrency protection, or recovery guarantees. Do not use real customer or financial data.
