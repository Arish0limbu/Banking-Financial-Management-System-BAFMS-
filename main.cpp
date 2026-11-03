#include <iostream>
#include "include/design.h"
#include "include/account.h"
#include "include/admin.h"
#include "include/customer.h"
#include "include/employee.h"
#include "include/finance.h"
#include "include/loan.h"
#include "include/security.h"
#include "include/transaction.h"

using namespace std;

int main()

{
    while (true)
    {
        AuthenticatedUser user;
        if (!signIn(user))
            return 0;
        if (user.role == UserRole::Administrator)
            adminmain();
        else
            customerPortal(user.customerSerial);
    }
}
