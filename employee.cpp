#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include "include/design.h"
#include "include/employee.h"

using namespace std;

static bool validEmployeeName(const string &name);
static bool validEmployeePhone(const string &phone);

static bool parseEmployeeInteger(const string &text, int &value)
{
    try
    {
        size_t consumed = 0;
        value = stoi(text, &consumed);
        return consumed == text.size();
    }
    catch (...)
    {
        return false;
    }
}

static bool parseEmployeeSalary(const string &text, double &value)
{
    try
    {
        size_t consumed = 0;
        value = stod(text, &consumed);
        return consumed == text.size() && isfinite(value);
    }
    catch (...)
    {
        return false;
    }
}

vector<EmployeeRecord> readEmployees()
{
    vector<EmployeeRecord> employees;
    ifstream file("data/employee.txt");
    string row;
    while (getline(file, row))
    {
        stringstream fields(row);
        string id, name, age, phone, role, salary, active;
        if (!getline(fields, id, '|') || !getline(fields, name, '|') ||
            !getline(fields, age, '|') || !getline(fields, phone, '|') ||
            !getline(fields, role, '|') || !getline(fields, salary, '|') ||
            !getline(fields, active) || (active != "0" && active != "1"))
            continue;

        EmployeeRecord employee{};
        if (!parseEmployeeInteger(id, employee.employeeId) ||
            !parseEmployeeInteger(age, employee.age) ||
            !parseEmployeeSalary(salary, employee.salary))
            continue;
        employee.name = name;
        employee.phone = phone;
        employee.role = role;
        employee.active = active == "1";

        const bool duplicateId = any_of(employees.begin(), employees.end(),
                                        [&employee](const EmployeeRecord &stored)
                                        { return stored.employeeId == employee.employeeId; });
        if (employee.employeeId > 0 && employee.age >= 18 && employee.age <= 90 &&
            employee.salary >= 0 && validEmployeeName(employee.name) &&
            validEmployeePhone(employee.phone) && !employee.role.empty() &&
            employee.role.find('|') == string::npos && !duplicateId)
            employees.push_back(employee);
    }
    return employees;
}

bool saveEmployees(const vector<EmployeeRecord> &employees)
{
    for (size_t i = 0; i < employees.size(); ++i)
    {
        const EmployeeRecord &employee = employees[i];
        if (employee.employeeId <= 0 || employee.age < 18 || employee.age > 90 ||
            !isfinite(employee.salary) || employee.salary < 0 ||
            !validEmployeeName(employee.name) || !validEmployeePhone(employee.phone) ||
            employee.role.empty() || employee.role.find_first_of("|\r\n") != string::npos)
            return false;

        for (size_t j = 0; j < i; ++j)
            if (employees[j].employeeId == employee.employeeId ||
                employees[j].phone == employee.phone)
                return false;
    }

    ofstream file("data/employee.txt", ios::trunc);
    if (!file.is_open())
        return false;
    file << fixed << setprecision(2);
    for (const EmployeeRecord &employee : employees)
        file << employee.employeeId << '|' << employee.name << '|' << employee.age << '|'
             << employee.phone << '|' << employee.role << '|' << employee.salary << '|'
             << (employee.active ? 1 : 0) << '\n';
    return file.good();
}

static bool validEmployeeName(const string &name)
{
    if (name.empty() || name.front() == ' ' || name.back() == ' ')
        return false;
    int words = 1;
    bool previousSpace = false;
    for (unsigned char ch : name)
    {
        if (isalpha(ch))
            previousSpace = false;
        else if (ch == ' ' && !previousSpace)
        {
            ++words;
            previousSpace = true;
        }
        else
            return false;
    }
    return words >= 2;
}

static bool validEmployeePhone(const string &phone)
{
    if (phone.size() != 10)
        return false;
    for (unsigned char ch : phone)
        if (!isdigit(ch))
            return false;
    return true;
}

static string titleCase(string text)
{
    bool capitalize = true;
    for (char &character : text)
    {
        unsigned char ch = static_cast<unsigned char>(character);
        if (character == ' ')
            capitalize = true;
        else
        {
            character = static_cast<char>(capitalize ? toupper(ch) : tolower(ch));
            capitalize = false;
        }
    }
    return text;
}

static void addEmployee()
{
    string name, phone, role;
    int age;
    double salary;
    cout << "Full name: ";
    getline(cin >> ws, name);
    if (!validEmployeeName(name))
    {
        cout << "Enter a full name using letters and single spaces.\n";
        ps();
        return;
    }
    if (!readint("Age: ", age, 18, 90))
        return;
    cout << "Phone number (10 digits): ";
    getline(cin, phone);
    if (!validEmployeePhone(phone))
    {
        cout << "Enter a 10 digit phone number.\n";
        ps();
        return;
    }
    cout << "Job title: ";
    getline(cin, role);
    if (role.empty() || role.find('|') != string::npos || role.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ") != string::npos)
    {
        cout << "Enter a job title using letters and spaces.\n";
        ps();
        return;
    }
    if (!readmoney("Monthly salary: ", salary, 0.0, 1000000000.0))
        return;

    vector<EmployeeRecord> employees = readEmployees();
    for (const EmployeeRecord &employee : employees)
        if (employee.phone == phone)
        {
            cout << "That phone number already belongs to an employee.\n";
            ps();
            return;
        }
    int nextId = 1;
    for (const EmployeeRecord &employee : employees)
        if (employee.employeeId >= nextId)
        {
            if (employee.employeeId == numeric_limits<int>::max())
            {
                cout << "No more employee IDs are available.\n";
                ps();
                return;
            }
            nextId = employee.employeeId + 1;
        }
    employees.push_back({nextId, titleCase(name), age, phone, titleCase(role), salary, true});
    if (saveEmployees(employees))
        cout << "Employee registered with ID " << nextId << ".\n";
    else
        cout << "Unable to save employee details.\n";
    ps();
}

static void listEmployees()
{
    heading("|| EMPLOYEE REGISTER ||");
    vector<EmployeeRecord> employees = readEmployees();
    if (employees.empty())
        cout << "No employees have been registered.\n";
    else
    {
        cout << left << setw(8) << "ID" << setw(26) << "NAME" << setw(8) << "AGE"
             << setw(14) << "PHONE" << setw(20) << "ROLE" << setw(14) << "SALARY" << "STATUS\n";
        line();
        cout << fixed << setprecision(2);
        for (const EmployeeRecord &employee : employees)
            cout << left << setw(8) << employee.employeeId << setw(26) << employee.name
                 << setw(8) << employee.age << setw(14) << employee.phone
                 << setw(20) << employee.role << setw(14) << employee.salary
                 << (employee.active ? "Active" : "Inactive") << '\n';
    }
    ps();
}

static void searchEmployee()
{
    int id;
    if (!readint("Employee ID: ", id, 1, 2147483647))
        return;
    for (const EmployeeRecord &employee : readEmployees())
        if (employee.employeeId == id)
        {
            cout << "ID: " << employee.employeeId << "\nName: " << employee.name
                 << "\nAge: " << employee.age << "\nPhone: " << employee.phone
                 << "\nRole: " << employee.role << "\nMonthly salary: "
                 << fixed << setprecision(2) << employee.salary
                 << "\nStatus: " << (employee.active ? "Active" : "Inactive") << '\n';
            ps();
            return;
        }
    cout << "Employee was not found.\n";
    ps();
}

static void updateSalary()
{
    int id;
    double salary;
    if (!readint("Employee ID: ", id, 1, 2147483647) ||
        !readmoney("New monthly salary: ", salary, 0.0, 1000000000.0))
        return;
    vector<EmployeeRecord> employees = readEmployees();
    for (EmployeeRecord &employee : employees)
        if (employee.employeeId == id && employee.active)
        {
            employee.salary = salary;
            cout << (saveEmployees(employees) ? "Salary updated.\n" : "Unable to save the salary update.\n");
            ps();
            return;
        }
    cout << "Active employee was not found.\n";
    ps();
}

static void deactivateEmployee()
{
    int id;
    if (!readint("Employee ID to deactivate: ", id, 1, 2147483647))
        return;
    vector<EmployeeRecord> employees = readEmployees();
    for (EmployeeRecord &employee : employees)
        if (employee.employeeId == id && employee.active)
        {
            employee.active = false;
            cout << (saveEmployees(employees) ? "Employee deactivated.\n" : "Unable to save employee status.\n");
            ps();
            return;
        }
    cout << "Active employee was not found.\n";
    ps();
}

void employeemanagement()
{
    int choice;
    do
    {
        heading("|| EMPLOYEE MANAGEMENT ||");
        cout << "1. Register employee\n2. View employee register\n3. Search employee\n"
             << "4. Update salary\n5. Deactivate employee\n6. Back\n";
        if (!readint("Select an option: ", choice, 1, 6))
            return;
        if (choice == 1)
            addEmployee();
        else if (choice == 2)
            listEmployees();
        else if (choice == 3)
            searchEmployee();
        else if (choice == 4)
            updateSalary();
        else if (choice == 5)
            deactivateEmployee();
    } while (choice != 6);
}
