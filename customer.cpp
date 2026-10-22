#include <iostream>
#include <algorithm>
#include <vector>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <cstdio>
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
    int words = 1;

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
            words++;
        }
        else
        {
            return false;
        }
    }

    return words >= 2;
}

string formatname(string name)
{
    bool first = true;

    for (char &ch : name)
    {
        if (ch == ' ')
        {
            first = true;
        }
        else
        {
            if (first)
                ch = toupper(static_cast<unsigned char>(ch));
            else
                ch = tolower(static_cast<unsigned char>(ch));

            first = false;
        }
    }

    return name;
}

string lowercase(string text)
{
    transform(text.begin(), text.end(), text.begin(),
              [](unsigned char ch)
              {
                  return tolower(ch);
              });

    return text;
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

struct Customer
{
    int sn;
    string name;
    int age;
    string address;
    string phone;
    string gmail;
};

bool validphone(long long phone)
{
    return phone > 9699999999LL && phone < 9900000000LL;
}

bool validphone(string phone)
{
    if (phone.length() != 10)
        return false;

    if (phone[0] != '9' || (phone[1] != '7' && phone[1] != '8'))
        return false;

    for (char ch : phone)
    {
        if (!isdigit(static_cast<unsigned char>(ch)))
            return false;
    }

    return true;
}

void filesave(string customername, string address, string gmail,
              int age, long long phone, int sn)
{
    vector<Customer> customers;
    string line;

    ifstream infile("data/customer.txt");

    while (getline(infile, line))
    {
        stringstream ss(line);
        Customer c;
        char separator1, separator2, separator3, separator4, separator5;

        if (ss >> c.sn >> separator1 &&
            separator1 == '|' &&
            getline(ss, c.name, '|') &&
            ss >> c.age >> separator2 &&
            separator2 == '|' &&
            getline(ss, c.address, '|') &&
            getline(ss, line, '|'))
        {
            stringstream phoneStream(line);

            if (!(phoneStream >> c.phone))
                continue;

            if (!getline(ss, c.gmail, '|'))
                continue;

            auto trim = [](string &s)
            {
                size_t first = s.find_first_not_of(" \t");
                if (first == string::npos)
                {
                    s.clear();
                    return;
                }

                size_t last = s.find_last_not_of(" \t");
                s = s.substr(first, last - first + 1);
            };

            trim(c.name);
            trim(c.address);
            trim(c.gmail);

            if (!c.name.empty())
                customers.push_back(c);
        }
    }

    infile.close();

    customers.push_back({sn, customername, age, address, to_string(phone), gmail});

    sort(customers.begin(), customers.end(),
         [](const Customer &a, const Customer &b)
         {
             string x = a.name;
             string y = b.name;

             transform(x.begin(), x.end(), x.begin(),
                       [](unsigned char ch)
                       { return tolower(ch); });

             transform(y.begin(), y.end(), y.begin(),
                       [](unsigned char ch)
                       { return tolower(ch); });

             return x < y;
         });

    ofstream file("data/customer.txt");

    if (!file)
        return;

    fileheader(file, "|| CUSTOMER DETAILS ||");
    filesubheader(file, 1, "SN");
    filesubheader(file, 8, "CUSTOMER NAME");
    filesubheader(file, 1, "AGE");
    filesubheader(file, 1, "ADDRESS (City-Ward,District,Country)");
    filesubheader(file, 1, "PHONE NUMBER");
    filesubheader(file, 9, "GMAIL ");
    fileline(file);

    sn = 1;

    for (const Customer &c : customers)
    {
        file << right << setw(3) << sn++ << right << setw(2) << "|";
        file << left << setw(29) << c.name << right << setw(1) << "|";
        file << right << setw(4) << c.age << right << setw(2) << "|";
        file << left << setw(37) << c.address << right << setw(2) << "|";
        file << left << setw(13) << c.phone << right << setw(2) << "|";
        file << left << setw(24) << c.gmail << right << setw(1) << "|" << endl;
    }
}

void newcustomer()
{

    string customername, address, gmail;
    char choice;
    int age, customers, sn = 1;
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
            customername = formatname(customername);

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

        ifstream file("data/customer.txt");

        if (file.peek() != EOF)
        {

            sn = 0;
            string dline;

            while (getline(file, dline))
            {
                size_t pos = dline.find('|');

                if (pos != string::npos)
                {
                    string value = dline.substr(0, pos);

                    try
                    {
                        sn = stoi(value);
                    }
                    catch (...)
                    {
                    }
                }
            }
            sn++;
            file.close();
        }

        filesave(customername, address, gmail, age, phone, sn);

        line();
        cout << "Do you want add another customer? Enter Y for Yes or N for NO (Y/N): ";
        cin >> choice;
        sn++;
    } while (choice == 'Y' || choice == 'y');

    customermanagement();
}

void viewcustomer()
{
    ifstream file("data/customer.txt");
    if (!file.is_open() || file.peek() == EOF)
    {
        cls();
        heading("|| CUSTOMER DETAILS ||");
        centertxt("File is empty OR didn't exits.....!");
        ps();
        customermanagement();
    }
    else
    {

        string details;

        cls();
        while (getline(file, details))
        {
            cout << details << endl;
        }

        file.close();
        line(120);
    }
    ps();
    customermanagement();
}

void searchcustomer()
{
    string name, phone, nline;
    bool found = false;
    cls();
    heading("|| CUSTOMER SEARCH RESULT ||", 120);
    cout << "Enter Customer Name: ";
    getline(cin >> ws, name);
    cout << "Enter Phone Number: ";
    getline(cin >> ws, phone);
    name = lowercase(name);
    ifstream file("data/customer.txt");
    if (!file.is_open())
    {
        centertxt("Unable to open customer file!", 120);
        ps();
        return;
    }
    if (file.peek() != EOF)
    {
        line(120);
        cout << " SN |        CUSTOMER NAME        | AGE | ADDRESS (City-Ward,District,Country) | PHONE NUMBER |         GMAIL          |" << endl;
        line(120);
    }
    while (getline(file, nline))
    {
        if (nline.find('|') == string::npos)
            continue;
        string lowerline = lowercase(nline);
        if (lowerline.find(name) != string::npos && nline.find(phone) != string::npos)
        {
            cout << nline << '\n';
            found = true;
        }
    }
    line(120);
    file.close();
    if (!found)
    {
        centertxt("Customer Not Found.....!", 120);
    }
    ps();
    customermanagement();
}

bool readCustomerLine(const string &line, Customer &c)
{
    size_t first = line.find('|');
    size_t second = line.find('|', first == string::npos ? first : first + 1);
    size_t third = line.find('|', second == string::npos ? second : second + 1);
    size_t fourth = line.find('|', third == string::npos ? third : third + 1);
    size_t fifth = line.find('|', fourth == string::npos ? fourth : fourth + 1);
    size_t sixth = line.find('|', fifth == string::npos ? fifth : fifth + 1);

    if (first == string::npos || second == string::npos ||
        third == string::npos || fourth == string::npos ||
        fifth == string::npos || sixth == string::npos)
        return false;

    try
    {
        string snText = line.substr(0, first);

        size_t start = snText.find_first_not_of(" \t");
        if (start == string::npos)
            return false;

        c.sn = stoi(snText.substr(start));

        c.name = line.substr(first + 1, second - first - 1);
        c.age = stoi(line.substr(second + 1, third - second - 1));
        c.address = line.substr(third + 1, fourth - third - 1);
        c.phone = line.substr(fourth + 1, fifth - fourth - 1);
        c.gmail = line.substr(fifth + 1, sixth - fifth - 1);

        auto trim = [](string &s)
        {
            size_t start = s.find_first_not_of(" \t");
            if (start == string::npos)
            {
                s.clear();
                return;
            }

            size_t end = s.find_last_not_of(" \t");
            s = s.substr(start, end - start + 1);
        };

        trim(c.name);
        trim(c.address);
        trim(c.phone);
        trim(c.gmail);

        return true;
    }
    catch (...)
    {
        return false;
    }
}

void updatecustomer()
{
    string targetName, targetPhone;

    heading("|| UPDATE CUSTOMER ||");

    ifstream file("data/customer.txt");

    if (!file.is_open())
    {
        centertxt("Unable to open customer file!............");
        ps();
        customermanagement();
        return;
    }

    do
    {

        cout << "Enter Customer Name: ";
        getline(cin >> ws, targetName);
        if (validname(targetName) == false)
        {
            footer("Invalid name!.Try full name!.RE-enter1.......... ");
            ps();
            heading("|| UPDATE CUSTOMER ||");
        }

    } while (!validname(targetName));

    do
    {
        cout << "Enter Customer Phone: ";
        getline(cin >> ws, targetPhone);
        if (validphone(targetPhone) == false)
        {
            footer("Enter only numbers!.Re-enter!..........");
            ps();
            heading("|| UPDATE CUSTOMER ||");
            cout << "Enter Customer Name:  " << targetName << endl;
        }

    } while (!validphone(targetPhone));

    vector<string> headerLines;
    vector<Customer> customers;
    string nline;

    while (getline(file, nline))
    {
        Customer c;
        if (readCustomerLine(nline, c))
        {
            customers.push_back(c);
        }
        else
        {
            headerLines.push_back(nline);
        }
    }

    file.close();

    bool found = false;
    int foundIndex = -1;

    for (size_t i = 0; i < customers.size(); i++)
    {
        if (lowercase(customers[i].name) == lowercase(targetName) && customers[i].phone == targetPhone)
        {
            found = true;
            foundIndex = i;
            footer("Customer Found!");
            cout << "SN      : " << customers[i].sn << endl;
            cout << "Name    : " << customers[i].name << '\n';
            cout << "Age     : " << customers[i].age << '\n';
            cout << "Address : " << customers[i].address << '\n';
            cout << "Phone   : " << customers[i].phone << '\n';
            cout << "Gmail   : " << customers[i].gmail << endl;

            int choice;
            footer("Option to Update!.");
            cout << "\n1. Update Name\n";
            cout << "2. Update Age\n";
            cout << "3. Update Address\n";
            cout << "4. Update Phone\n";
            cout << "5. Update Gmail\n";
            cout << "6. Update All Details\n";
            cout << "7. Cancel\n";
            line();
            cout << "Enter number to select that option. Which u want to update: ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                ps();
                customermanagement();
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string input;
            int ageInput;

            switch (choice)
            {
            case 1:
                do
                {
                    cout << "Enter New Name: ";
                    getline(cin, input);
                } while (!validname(input));

                customers[i].name = input;
                break;

            case 2:
                do
                {
                    cout << "Enter New Age: ";

                    if (!(cin >> ageInput))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        ageInput = 0;
                    }
                } while (!validage(ageInput));

                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                customers[i].age = ageInput;
                break;

            case 3:
                do
                {
                    cout << "Enter New Address: ";
                    getline(cin, input);
                } while (!validaddress(input));

                customers[i].address = input;
                break;

            case 4:
                do
                {
                    cout << "Enter New Phone: ";
                    getline(cin, input);
                } while (!validphone(input));

                customers[i].phone = input;
                break;

            case 5:
                do
                {
                    cout << "Enter New Gmail: ";
                    getline(cin, input);
                } while (!validgmail(input));

                customers[i].gmail = input;
                break;

            case 6:
                do
                {
                    cout << "Enter New Name: ";
                    getline(cin, customers[i].name);
                } while (!validname(customers[i].name));

                do
                {
                    cout << "Enter New Age: ";

                    if (!(cin >> customers[i].age))
                    {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        customers[i].age = 0;
                    }
                } while (!validage(customers[i].age));

                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                do
                {
                    cout << "Enter New Address: ";
                    getline(cin, customers[i].address);
                } while (!validaddress(customers[i].address));

                do
                {
                    cout << "Enter New Phone: ";
                    getline(cin, customers[i].phone);
                } while (!validphone(customers[i].phone));

                do
                {
                    cout << "Enter New Gmail: ";
                    getline(cin, customers[i].gmail);
                } while (!validgmail(customers[i].gmail));

                break;

            case 7:
                cout << "Update cancelled.\n";
                ps();
                customermanagement();
                return;

            default:
                cout << "Invalid choice.\n";
                ps();
                customermanagement();
                return;
            }

            cout << "\nCustomer updated successfully.\n";
            ps();

            ofstream outfile("data/customer.txt");

            if (!outfile.is_open())
            {
                cout << "Unable to open customer file for writing.\n";
                ps();
                customermanagement();
                return;
            }

            for (const string &header : headerLines)
            {
                outfile << header << '\n';
            }

            sort(customers.begin(), customers.end(),
                 [](const Customer &a, const Customer &b)
                 {
                     string x = a.name;
                     string y = b.name;

                     transform(x.begin(), x.end(), x.begin(),
                               [](unsigned char ch)
                               { return tolower(ch); });

                     transform(y.begin(), y.end(), y.begin(),
                               [](unsigned char ch)
                               { return tolower(ch); });

                     return x < y;
                 });

            int sn = 1;
            for (const Customer &c : customers)
            {
                outfile << right << setw(3) << sn++ << right << setw(2) << "|";
                outfile << left << setw(29) << c.name << right << setw(1) << "|";
                outfile << right << setw(4) << c.age << right << setw(2) << "|";
                outfile << left << setw(37) << c.address << right << setw(2) << "|";
                outfile << left << setw(13) << c.phone << right << setw(2) << "|";
                outfile << left << setw(24) << c.gmail << right << setw(1) << "|" << endl;
            }

            outfile.close();
            customermanagement();
            break;
        }
    }

    if (!found)
    {
        cout << "\nCustomer not found.\n";
        ps();
        customermanagement();
        return;
    }
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
