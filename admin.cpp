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
             << "7. Logout\n";

        if (!readint("Select an option: ", choice, 1, 7))
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
            cout << "You have been logged out.\n";
            return;
        }
    }
}
