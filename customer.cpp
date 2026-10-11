#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "include/customer.h"
#include "design.h"

using namespace std;

const string CUSTOMER_FILE = "data/customers.txt";

bool readcustomer(ifstream &file, Customer &c)
{
    
}

void newcustomer()
{
    heading("|| Register Customer ||");
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