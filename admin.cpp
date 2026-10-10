#include <iostream>
#include <limits>
#include <conio.h>
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

void adminmain()
{
    int choice;
    heading("|| WELCOME TO ADMIN PAGE ||");
    cout << "1. Customer & Account Management." << endl
         << "2. Transactuion mangement." << endl
         << "3. Loan Management." << endl
         << "4. Employee Management." << endl
         << "5. Reports & Analytics." << endl
         << "6. Admin Settings." << endl
         << "7. Logout." << endl;
    line();
    do
    {
        cout << "Enter number to select the option: ";
        cin >> choice;
        if (cin.fail())
        {
            footer(" Invalid input! Enter integers only. ");
            ps();
            cin.clear();
            continue;
        }
        if (choice < 1 || choice > 7)
        {
            footer(" Enter given option number from 1 to 7. ");
        }

    } while (cin.fail() || choice < 1 || choice > 7);
}