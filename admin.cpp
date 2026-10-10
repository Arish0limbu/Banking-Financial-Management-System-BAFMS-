#include <iostream>
#include <conio.h>
#include "include/admin.h"
#include "include/design.h"
#include <string>

using namespace std;

string hidepass()
{
    string pass;
    char ch;

    while ((ch = _getch()) != 13)
    {
        if (ch == 8)
        {
            if (!pass.empty())
            {
                pass.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch != 0 && ch != 224)
        {
            pass += ch;
            cout << '*';
        }
    }

    cout << endl;
    return pass;
}

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
        pass = hidepass();

        if (id == "admin" && pass == "123")
        {
            footer("SUCESS!");
            ps();
            return true;
        }
        else
        {
            count--;
            if (count > 0)

            {
                footer("ID or Password is incorrect!");
                centertxt(to_string(count) + " Attempts left!....");
                ps();
            }
            else
            {
                footer("Login faild! No more attemps left!");
                ps();
                return false;
            }
        }
    } while (count != 0);
}