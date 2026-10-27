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

#include "include/account.h"
#include "include/customer.h"
#include "include/design.h"
#include "include/loan.h"

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

bool readCustomerLine(const string &line, Customer &c);

static bool customerNameLess(const Customer &a, const Customer &b)
{
    return lowercase(a.name) < lowercase(b.name);
}

static bool writeCustomerFile(vector<Customer> customers)
{
    sort(customers.begin(), customers.end(), customerNameLess);

    ofstream file("data/customer.txt", ios::trunc);
    if (!file.is_open())
        return false;

    fileheader(file, "|| CUSTOMER DETAILS ||");
    filesubheader(file, 1, "SN");
    filesubheader(file, 8, "CUSTOMER NAME");
    filesubheader(file, 1, "AGE");
    filesubheader(file, 1, "ADDRESS (City-Ward,District,Country)");
    filesubheader(file, 1, "PHONE NUMBER");
    filesubheader(file, 9, "GMAIL ");
    fileline(file);

    for (const Customer &customer : customers)
    {
        file << right << setw(3) << customer.sn << right << setw(2) << "|";
        file << left << setw(29) << customer.name << right << setw(1) << "|";
        file << right << setw(4) << customer.age << right << setw(2) << "|";
        file << left << setw(37) << customer.address << right << setw(2) << "|";
        file << left << setw(13) << customer.phone << right << setw(2) << "|";
        file << left << setw(24) << customer.gmail << right << setw(1) << "|" << '\n';
    }

    return file.good();
}

static int nextCustomerSerial()
{
    ifstream file("data/customer.txt");
    string row;
    int highestSerial = 0;
    while (getline(file, row))
    {
        Customer customer{};
        if (readCustomerLine(row, customer))
            highestSerial = max(highestSerial, customer.sn);
    }
    return highestSerial + 1;
}

static bool customerPhoneExists(const string &phone, int exceptSerial = 0)
{
    ifstream file("data/customer.txt");
    string row;
    while (getline(file, row))
    {
        Customer customer{};
        if (readCustomerLine(row, customer) && customer.sn != exceptSerial && customer.phone == phone)
            return true;
    }
    return false;
}

static bool customerEmailExists(const string &gmail, int exceptSerial = 0)
{
    ifstream file("data/customer.txt");
    string row;
    while (getline(file, row))
    {
        Customer customer{};
        if (readCustomerLine(row, customer) && customer.sn != exceptSerial &&
            lowercase(customer.gmail) == lowercase(gmail))
            return true;
    }
    return false;
}

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
        Customer customer{};
        if (readCustomerLine(line, customer))
            customers.push_back(customer);
    }
    customers.push_back({sn, customername, age, address, to_string(phone), gmail});
    writeCustomerFile(customers);
}

void newcustomer()
{

    string customername, address, gmail;
    char choice;
    int age, sn = 1;
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
            else if (customerEmailExists(gmail))
            {
                footer("That Gmail address already belongs to a customer.");
                ps();
            }
        } while (validgmail(gmail) == false || customerEmailExists(gmail));

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
            else if (customerPhoneExists(to_string(phone)))
            {
                footer("That phone number already belongs to a customer.");
                ps();
            }

        } while (validphone(phone) == false || customerPhoneExists(to_string(phone)));

        sn = nextCustomerSerial();
        filesave(customername, address, gmail, age, phone, sn);

        line();
        cout << "Do you want add another customer? Enter Y for Yes or N for NO (Y/N): ";
        cin >> choice;
        sn++;
    } while (choice == 'Y' || choice == 'y');

}

void viewcustomer()
{
    ifstream file("data/customer.txt");
    if (!file.is_open() || file.peek() == EOF)
    {
        cls();
        heading("|| CUSTOMER DETAILS ||");
        centertxt("File is empty OR didn't exits.....!");
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
            continue;
    }

    file.close();

    bool found = false;

    for (size_t i = 0; i < customers.size(); i++)
    {
        if (lowercase(customers[i].name) == lowercase(targetName) && customers[i].phone == targetPhone)
        {
            found = true;
            footer("Customer Found!");
            cout << "SN      : " << customers[i].sn << endl;
            cout << "Name    : " << customers[i].name << '\n';
            cout << "Age     : " << customers[i].age << '\n';
            cout << "Address : " << customers[i].address << '\n';
            cout << "Phone   : " << customers[i].phone << '\n';
            cout << "Gmail   : " << customers[i].gmail << endl;

            int choice;
            footer("Option to Update!.");
            cout << "1. Update Name\n";
            cout << "2. Update Age\n";
            cout << "3. Update Address\n";
            cout << "4. Update Phone\n";
            cout << "5. Update Gmail\n";
            cout << "6. Update All Details\n";
            cout << "7. Cancel\n";
            line();
            if (!readint("Enter update option (1-7): ", choice, 1, 7))
            {
                return;
            }

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

                customers[i].name = formatname(input);
                break;

            case 2:
                if (!readint("Enter New Age: ", ageInput, 18, 90))
                    return;
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
                customers[i].name = formatname(customers[i].name);

                if (!readint("Enter New Age: ", customers[i].age, 18, 90))
                    return;

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
                return;

            default:
                cout << "Invalid choice.\n";
                ps();
                return;
            }

            if (customerPhoneExists(customers[i].phone, customers[i].sn))
            {
                cout << "That phone number already belongs to another customer.\n";
                ps();
                return;
            }
            if (customerEmailExists(customers[i].gmail, customers[i].sn))
            {
                cout << "That Gmail address already belongs to another customer.\n";
                ps();
                return;
            }

            if (!writeCustomerFile(customers))
            {
                cout << "Unable to save customer changes.\n";
                ps();
                return;
            }
            cout << "\nCustomer updated successfully.\n";
            ps();
            return;
        }
    }

    if (!found)
    {
        cout << "\nCustomer not found.\n";
        ps();
        return;
    }
}

void deletecustomer()
{
    string targetName, targetPhone, currentLine;
    vector<Customer> customers;

    heading("|| DELETE CUSTOMER ||");
    do
    {
        cout << "Enter Customer Name: ";
        getline(cin >> ws, targetName);
        if (!validname(targetName))
            cout << "Enter the customer's full name.\n";
    } while (!validname(targetName));

    do
    {
        cout << "Enter Customer Phone: ";
        getline(cin, targetPhone);
        if (!validphone(targetPhone))
            cout << "Enter a valid 10 digit phone number.\n";
    } while (!validphone(targetPhone));

    ifstream file("data/customer.txt");
    if (!file.is_open())
    {
        cout << "Unable to open customer file.\n";
        ps();
        return;
    }

    while (getline(file, currentLine))
    {
        Customer customer;
        if (readCustomerLine(currentLine, customer))
            customers.push_back(customer);
        else
            continue;
    }
    file.close();

    auto match = customers.end();
    for (auto it = customers.begin(); it != customers.end(); ++it)
    {
        if (lowercase(it->name) == lowercase(targetName) && it->phone == targetPhone)
        {
            match = it;
            break;
        }
    }

    if (match == customers.end())
    {
        cout << "Customer not found.\n";
        ps();
        return;
    }

    for (const AccountRecord &account : readAccounts())
    {
        if (account.customerSerial == match->sn)
        {
            cout << "This customer has account records. Close or retain those records before deletion.\n";
            ps();
            return;
        }
    }

    for (const LoanRecord &loan : readLoans())
    {
        if (loan.customerSerial == match->sn)
        {
            cout << "This customer has loan history and cannot be deleted.\n";
            ps();
            return;
        }
    }

    cout << "Customer: " << match->name << " | Phone: " << match->phone << '\n';
    char confirm;
    cout << "Delete this customer? (Y/N): ";
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "Deletion cancelled.\n";
        ps();
        return;
    }

    customers.erase(match);
    if (!writeCustomerFile(customers))
    {
        cout << "Unable to save customer file.\n";
        ps();
        return;
    }

    cout << "Customer deleted successfully.\n";
    ps();
}

void customermanagement()
{
    int choice;
    while (true)
    {
        heading("|| Customer Management ||");
        cout << "1. Register new customer\n"
             << "2. View all customers\n"
             << "3. Search customer\n"
             << "4. Update customer details\n"
             << "5. Delete customer\n"
             << "6. Back\n";
        if (!readint("Select an option: ", choice, 1, 6))
            return;
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
            return;
        }
    }
}
