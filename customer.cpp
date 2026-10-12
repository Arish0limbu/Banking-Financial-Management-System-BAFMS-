#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <windows.h>

#include "include/customer.h"
#include "design.h"

using namespace std;

void readcustomerfile()
{
    string path;

    path += "data/customer.txt";

    ifstream file(path);

    if (file.is_open())
    {
        file.close();
    }
    else
    {
        ofstream newfile(path);
    }
}

void newcustomer()
{

    string customername, address, gmail;
    char choice;
    int age, customers, phone;

    readcustomerfile();
    do
    {
        heading("|| Register Customer ||");
        cout << "Enter Information of customer." << endl;
        cout << "Full Name: ";
        getline(cin >> ws, customername);
        cout << "Age: ";
        cin >> age;
        cout << "Address: ";
        getline(cin >> ws, address);
        cout << "Gmail: ";
        getline(cin >> ws, gmail);
        cout << "Phone Number: ";
        cin >> phone;

        ofstream file("data/student.txt", ios::app);

        file << customername;
        file << age;
        file << address;
        file << gmail;
        file << phone;

        line();
        cout << "Do you want add another customer? Enter Y for Yes or N for NO (Y/N): ";
        cin >> choice;
    } while (choice == 'Y' || choice == 'y');
}

void viewcustomer()
{
}

void searchcustomer()
{
}

void updatecustomer()
{
}

void deletecustomer()
{
}

void customermanagement()
{
    int choice;
    do
    {
        heading("|| Customer Management ||");
        cout << "1. Register new Customer. " << endl
             << "2. View all customer." << endl
             << "3. Search customer." << endl
             << "4. Update Customer Details." << endl
             << "5. Delete Customer." << endl
             << "6. Back." << endl;

        line();
        cout << "Enter number to select the option: ";
        cin >> choice;
        if (cin.fail())
        {
            footer(" Invalid input! Enter integers only. ");
            ps();
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }
        if (choice < 1 || choice > 6)
        {
            footer(" Enter given option number from 1 to 6. ");
            ps();
            continue;
        }

    } while (cin.fail() || choice < 1 || choice > 6);
    switch (choice)
    {
    case 1:
        newcustomer();
        break;
    case 2:
        viewcustomer();
        break;
    case 3:
        searchcustomer();
        break;
    case 4:
        updatecustomer();
        break;
    case 5:
        deletecustomer();
        break;
    case 6:
        break;
    }
}