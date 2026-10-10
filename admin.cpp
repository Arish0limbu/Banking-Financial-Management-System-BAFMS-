#include <iostream>
#include "include/admin.h"
#include "include/design.h"
#include <string>

using namespace std;

bool adminlogin()
{
    string id, pass;

    heading("|| Admin Login Page ||");
    cout << "Enter ID: ";
    getline(cin, id);
    cout << "Enter Pass: ";
    getline(cin, pass);

    if (id == "admin" && pass == "123")
    {
        return true;
    }
    else
    {
        heading("ID or Password is incorrect!");
        return false;
    }
}