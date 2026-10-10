#include <iostream>
#include "include/design.h"
#include "include/admin.h"

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