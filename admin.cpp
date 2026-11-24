#include <iostream>
#include "include/account.h"
#include "include/admin.h"
#include "include/customer.h"
#include "include/design.h"
#include "include/employee.h"
#include "include/finance.h"
#include "include/loan.h"
#include "include/security.h"
#include "include/transaction.h"

using namespace std;

static void customeraccountmenu()
{
    int choice;
    do
    {
        heading("|| CUSTOMER & ACCOUNT MANAGEMENT ||");
        cout << "1. Customer management\n2. Account management\n3. Back to admin menu\n";
        if (!readint("Select an option: ", choice, 1, 3))
            return;
        if (choice == 1)
            customermanagement();
        else if (choice == 2)
            accountmanagement();
    } while (choice != 3);
}

static void adminsettings()
{
    int choice;
    do
    {
        heading("|| ADMIN SETTINGS ||");
        cout << "1. Change password\n2. View administrator profile\n3. Back\n";
        if (!readint("Select an option: ", choice, 1, 3))
            return;
        if (choice == 1)
            changeAdminPassword();
        else if (choice == 2)
            showAdminProfile();
    } while (choice != 3);
}

static void exportdatamenu()
{
    int choice;
    do
    {
        heading("|| DATA EXPORTS ||");
        cout << "1. Monthly analytics CSV\n2. Payroll by role CSV\n"
             << "3. Loan portfolio CSV\n4. Customer records CSV\n"
             << "5. Account register CSV\n6. Transaction ledger CSV\n"
             << "7. Employee register CSV\n8. Filtered transaction CSV\n"
             << "9. Filtered loan portfolio CSV\n10. Filtered account register CSV\n"
             << "11. Filtered employee register CSV\n12. Accounts for one customer CSV\n"
             << "13. Loans for one customer CSV\n14. Employees by job role CSV\n"
             << "15. Transactions for one account CSV\n16. Back\n";
        if (!readint("Select an option: ", choice, 1, 16))
            return;
        if (choice == 1)
            cout << (exportMonthlyAnalyticsCsv()
                         ? "Monthly analytics saved to data/monthly_analytics.csv.\n"
                         : "Unable to export monthly analytics. Check the data folder.\n");
        else if (choice == 2)
            cout << (exportPayrollByRoleCsv()
                         ? "Payroll summary saved to data/payroll_by_role.csv.\n"
                         : "Unable to export payroll summary. Check the data folder.\n");
        else if (choice == 3)
            cout << (exportLoanPortfolioCsv()
                         ? "Loan portfolio saved to data/loan_portfolio.csv.\n"
                         : "Unable to export the loan portfolio. Check the data folder.\n");
        else if (choice == 4)
            cout << (exportCustomerRecordsCsv()
                         ? "Customer records saved to data/customer_records.csv.\n"
                         : "Unable to export customer records. Check the data folder.\n");
        else if (choice == 5)
            cout << (exportAccountsCsv()
                         ? "Account register saved to data/account_register.csv.\n"
                         : "Unable to export the account register. Check the data folder.\n");
        else if (choice == 6)
            cout << (exportTransactionsCsv()
                         ? "Transaction ledger saved to data/transaction_ledger.csv.\n"
                         : "Unable to export the transaction ledger. Check the data folder.\n");
        else if (choice == 7)
            cout << (exportEmployeesCsv()
                         ? "Employee register saved to data/employee_register.csv.\n"
                         : "Unable to export the employee register. Check the data folder.\n");
        else if (choice == 8)
            cout << (exportFilteredTransactionsCsv()
                         ? "Filtered ledger saved to data/filtered_transaction_ledger.csv.\n"
                         : "Unable to export the filtered ledger. Check the selected filters and data folder.\n");
        else if (choice == 9)
            cout << (exportFilteredLoanPortfolioCsv()
                         ? "Filtered loan portfolio saved to data/filtered_loan_portfolio.csv.\n"
                         : "Unable to export the filtered loan portfolio. Check the data folder.\n");
        else if (choice == 10)
            cout << (exportFilteredAccountsCsv()
                         ? "Filtered account register saved to data/filtered_account_register.csv.\n"
                         : "Unable to export the filtered account register. Check the data folder.\n");
        else if (choice == 11)
            cout << (exportFilteredEmployeesCsv()
                         ? "Filtered employee register saved to data/filtered_employee_register.csv.\n"
                         : "Unable to export the filtered employee register. Check the data folder.\n");
        else if (choice == 12)
            cout << (exportCustomerAccountsCsv()
                         ? "Customer account register exported.\n"
                         : "Unable to export customer accounts. Check the ID and data folder.\n");
        else if (choice == 13)
            cout << (exportCustomerLoanPortfolioCsv()
                         ? "Customer loan portfolio exported.\n"
                         : "Unable to export customer loans. Check the ID and data folder.\n");
        else if (choice == 14)
            cout << (exportEmployeesByRoleCsv()
                         ? "Role-filtered employee register saved to data/role_filtered_employee_register.csv.\n"
                         : "Unable to export employees for that role. Check the data folder.\n");
        else if (choice == 15)
            cout << (exportAccountTransactionsCsv()
                         ? "Account transaction ledger exported.\n"
                         : "Unable to export transactions for that account. Check the account number and data folder.\n");
        if (choice != 16)
            ps();
    } while (choice != 16);
}

void adminmain()
{
    int choice;
    while (true)
    {
        heading("|| WELCOME TO ADMIN PAGE ||");
        cout << "1. Customer & account management\n"
             << "2. Transaction management\n"
             << "3. Loan management\n"
             << "4. Employee management\n"
             << "5. Reports & analytics\n"
             << "6. Admin settings\n"
             << "7. Account reconciliation\n"
             << "8. Data exports\n"
             << "9. Logout\n";

        if (!readint("Select an option: ", choice, 1, 9))
            return;
        switch (choice)
        {
        case 1:
            customeraccountmenu();
            break;
        case 2:
            transactionmanagement();
            break;
        case 3:
            loanmanagement();
            break;
        case 4:
            employeemanagement();
            break;
        case 5:
            showReports();
            break;
        case 6:
            adminsettings();
            break;
        case 7:
            showAccountReconciliation();
            break;
        case 8:
            exportdatamenu();
            break;
        case 9:
            cout << "You have been logged out.\n";
            return;
        }
    }
}
