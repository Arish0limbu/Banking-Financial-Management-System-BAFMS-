#include <iostream>
#include "include/design.h"
#include "include/account.h"
#include "include/admin.h"
#include "include/customer.h"
#include "include/employee.h"
#include "include/finance.h"
#include "include/loan.h"
#include "include/transaction.h"

using namespace std;

int main()

{
    int choice;
    while (true)
    {
        heading("|| BANKING & FINANCIAL MANAGEMENT SYSTEM ||");
        cout << "1. Administrator login\n2. Customer login\n3. Exit\n";
        if (!readint("Select an option: ", choice, 1, 3))
            return 0;
        if (choice == 1)
        {
            if (adminlogin())
                adminmain();
        }
        else if (choice == 2)
        {
            int customerSerial;
            if (customerlogin(customerSerial))
                customerPortal(customerSerial);
        }
        else
            return 0;
    }
}
