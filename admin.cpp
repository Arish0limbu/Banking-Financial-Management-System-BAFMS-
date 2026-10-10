#include <iostream>
#include "include/admin.h"
#include "include/design.h"
#include <string>

using namespace std;

bool adminlogin()
{
    string id, pass;
    int count = 4;

    do
    {
        heading("|| Admin Login Page ||");
        cout << "Enter ID: ";
        getline(cin, id);
        cout << "Enter Pass: ";
        getline(cin, pass);

        if (id == "admin" && pass == "123")
        {
            footer("SUCESS!");
            return true;
        }
        else
        {
            if (count != 0)

            {
                footer("ID or Password is incorrect!");
                count--;
                cout << count;
                ps();
            }
            else
            {
                return false;
            }
        }
    } while (count != 0);
}