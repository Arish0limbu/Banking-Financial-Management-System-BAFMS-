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
    if (adminlogin())
    {
        adminmain();
    }
    else
    {
        return 0;
    }
    return 0;
}