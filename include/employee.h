#ifndef BAFMS_EMPLOYEE_H
#define BAFMS_EMPLOYEE_H

#include <string>
#include <vector>

struct EmployeeRecord
{
    int employeeId;
    std::string name;
    int age;
    std::string phone;
    std::string role;
    double salary;
    bool active;
};

std::vector<EmployeeRecord> readEmployees();
bool saveEmployees(const std::vector<EmployeeRecord> &employees);
void employeemanagement();

#endif
