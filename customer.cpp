#include <iostream>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <windows.h>

#include "include/customer.h"
#include "include/design.h"

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

bool validname(string name)
{
    if (name.empty() || name[0] == ' ' || name.back() == ' ')
        return false;
    bool space = false;

    for (char ch : name)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
        {
            space = false;
        }

        else if (ch == ' ')
        {
            if (space)
                return false;

            space = true;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool validage(int age)
{
    if (age < 18 || age > 90)
    {
        return false;
    }

    return true;
}

bool validaddress(string address)
{

    if (address.empty())
        return false;

    if (address.front() == ' ' || address.back() == ' ')
        return false;

    for (int i = 0; i < address.length(); i++)
    {
        if (address[i] == ' ' && address[i + 1] == ' ')
            return false;
    }

    int commaCount = 0;

    for (char ch : address)
    {
        if (ch == ',')
            commaCount++;
    }

    if (commaCount != 2)
        return false;

    int firstComma = address.find(',');
    int secondComma = address.find(',', firstComma + 1);

    string cityWard = address.substr(0, firstComma);
    string district = address.substr(firstComma + 1, secondComma - firstComma - 1);
    string country = address.substr(secondComma + 1);

    if (cityWard.empty() || district.empty() || country.empty())
        return false;

    auto trim = [](string &s)
    {
        while (!s.empty() && s.front() == ' ')
            s.erase(s.begin());

        while (!s.empty() && s.back() == ' ')
            s.pop_back();
    };

    trim(cityWard);
    trim(district);
    trim(country);

    int hyphen = cityWard.find('-');

    if (hyphen == string::npos || hyphen == 0 || hyphen == cityWard.length() - 1)
        return false;

    string city = cityWard.substr(0, hyphen);
    string ward = cityWard.substr(hyphen + 1);

    bool previousSpace = false;

    for (char ch : city)
    {
        if (isalpha(static_cast<unsigned char>(ch)))
            previousSpace = false;
        else if (ch == ' ')
        {
            if (previousSpace)
                return false;

            previousSpace = true;
        }
        else
            return false;
    }

    if (ward.empty())
        return false;

    for (char ch : ward)
    {
        if (!isdigit(static_cast<unsigned char>(ch)))
            return false;
    }

    string parts[] = {district, country};

    for (string part : parts)
    {
        if (part.empty())
            return false;

        previousSpace = false;

        for (char ch : part)
        {
            if (isalpha(static_cast<unsigned char>(ch)))
                previousSpace = false;
            else if (ch == ' ')
            {
                if (previousSpace)
                    return false;

                previousSpace = true;
            }
            else
                return false;
        }
    }

    return true;
}

bool validgmail(string gmail)
{
    if (gmail.empty() || gmail.find(' ') != string::npos)
        return false;

    if (gmail.find('@') == string::npos || gmail.find('@') != gmail.rfind('@'))
        return false;

    size_t at = gmail.find('@');

    if (at == 0 || gmail.substr(at) != "@gmail.com")
        return false;

    if (gmail[0] == '.' || gmail[at - 1] == '.')
        return false;

    if (gmail.find("..") != string::npos)
        return false;

    for (size_t i = 0; i < at; i++)
    {
        char ch = gmail[i];

        if (!isalnum(static_cast<unsigned char>(ch)) && ch != '.' &&
            ch != '_' && ch != '%' && ch != '+' && ch != '-')
            return false;
    }

    return true;
}

bool validphone(long long phone)
{
    return phone > 9699999999LL && phone < 9900000000LL;
}

void filesave(string customername, string address, string gmail, int age, long long phone)
{
    int sn = 1;
    string lengthcount[7];
    readcustomerfile();
    ofstream file("data/customer.txt", ios::app);
    if (!file)
        return;
    file.seekp(0, ios::end);
    if (file.tellp() == 0)
    {
        fileheader(file, "|| CUSTOMER DETAILS ||");
        filesubheader(file, 1, "SN");
        filesubheader(file, 8, "CUSTOMER NAME");
        filesubheader(file, 1, "AGE");
        filesubheader(file, 1, "ADDRESS (City-Ward,District,Country)");
        filesubheader(file, 1, "PHONE NUMBER");
        filesubheader(file, 8, "GMAIL");
        fileline(file);
        sn = 1;
    }
    file << right << setw(3) << sn << right << setw(2) << "|";
    file << left << setw(21) << customername << right << setw(9) << "|";
    file << right << setw(4) << age << right << setw(2) << "|";
    file << left << setw(37) << address << right << setw(2) << "|";
    file << left << setw(13) << phone << right << setw(2) << "|";
    file << left << setw(15) << gmail << right << setw(3) << "|";
}

void newcustomer()
{

    string customername, address, gmail;
    char choice;
    int age, customers;
    long long phone;

    readcustomerfile();
    do
    {
        do
        {
            heading("|| Register Customer ||");
            cout << "Enter Information of customer." << endl;
            cout << "Full Name: ";
            getline(cin >> ws, customername);

            if (validname(customername) == false)
            {
                footer("Invalid Name!. Re Enter the Name.");
                ps();
            }
        } while (validname(customername) == false);

        do
        {
            cout << "Age: ";
            cin >> age;
            if (validage(age) == false)
            {
                footer("Age is invalid!. Re Enter. ");
                ps();
                cin.clear();
                cin.ignore(10000, '\n');

                heading("|| Register Customer ||");
                cout << "Enter Information of customer." << endl;
                cout << "Full Name: " << customername << endl;
            }

        } while (validage(age) == false);

        do
        {

            cout << "Address (City-Ward no,District,Country): ";
            getline(cin >> ws, address);
            if (validaddress(address) == false)
            {
                footer("Address is Invalid!. Re Enter as format.");
                ps();

                heading("|| Register Customer ||");
                cout << "Enter Information of customer." << endl;
                cout << "Full Name: " << customername << endl;
                cout << "Age: " << age << endl;
            }

        } while (validaddress(address) == false);

        do
        {
            cout << "Gmail: ";
            getline(cin >> ws, gmail);
            if (validgmail(gmail) == false)
            {
                footer("Gmail is Invalid!. Re Enter.");
                ps();

                heading("|| Register Customer ||");
                cout << "Enter Information of customer." << endl;
                cout << "Full Name: " << customername << endl;
                cout << "Age: " << age << endl;
                cout << "Address (City-Ward no,District,Country): " << address << endl;
            }
        } while (validgmail(gmail) == false);

        do
        {
            cout << "Phone Number: ";

            if (!(cin >> phone))
            {
                cin.clear();
                cin.ignore(100000, '\n');

                footer("Invalid input!. Enter numbers only.");
                ps();

                heading("|| Register Customer ||");
                cout << "Enter Information of customer." << endl;
                cout << "Full Name: " << customername << endl;
                cout << "Age: " << age << endl;
                cout << "Address (City-Ward no, District, Country): " << address << endl;
                cout << "Gmail: " << gmail << endl;

                phone = 0;
                continue;
            }

            cin.ignore(100000, '\n');

            if (validphone(phone) == false)
            {
                footer("Phone number is Invalid!. Re-enter.");
                ps();

                heading("|| Register Customer ||");
                cout << "Enter Information of customer." << endl;
                cout << "Full Name: " << customername << endl;
                cout << "Age: " << age << endl;
                cout << "Address (City-Ward no, District, Country): " << address << endl;
                cout << "Gmail: " << gmail << endl;
            }

        } while (validphone(phone) == false);

        filesave(customername, address, gmail, age, phone);

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
