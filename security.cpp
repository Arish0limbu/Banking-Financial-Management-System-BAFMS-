#include <cerrno>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#ifdef _WIN32
#include <conio.h>
#endif
#include "include/customer.h"
#include "include/design.h"
#include "include/security.h"
#include <algorithm>
#include <cctype>

using namespace std;

static pair<string, string> readCredentials()
{
    const string path = "data/admin.txt";
    ifstream file(path);
    if (!file.is_open())
    {
        errno = 0;
        FILE *probe = fopen(path.c_str(), "rb");
        if (probe)
            fclose(probe);
        else if (errno == ENOENT)
            return {"admin", "123"};
        return {"", ""};
    }

    string id, password;
    if (getline(file, id, '|') && getline(file, password) &&
        !id.empty() && !password.empty())
        return {id, password};
    return {"", ""};
}

string readPassword(const string &prompt)
{
    cout << prompt;
#ifdef _WIN32
    string password;
    while (true)
    {
        int key = _getch();
        if (key == 0 || key == 224)
        {
            _getch();
            continue;
        }
        if (key == 13)
            break;
        if (key == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b";
            }
        }
        else if (key >= 32 && key <= 126)
        {
            password.push_back(static_cast<char>(key));
            cout << '*';
        }
    }
    cout << '\n';
    return password;
#else
    string password;
    getline(cin, password);
    return password;
#endif
}

bool authenticateAdministrator(const string &id, const string &password)
{
    const pair<string, string> credentials = readCredentials();
    return !credentials.first.empty() && !credentials.second.empty() &&
           id == credentials.first && password == credentials.second;
}

LoginResult authenticateUser(const string &id, const string &password,
                             AuthenticatedUser &user)
{
    if (authenticateAdministrator(id, password))
    {
        user.role = UserRole::Administrator;
        user.customerSerial = 0;
        return LoginResult::Authenticated;
    }

    int customerSerial = 0;
    const CustomerAuthentication customerResult =
        authenticateCustomer(id, password, customerSerial);
    if (customerResult == CustomerAuthentication::Authenticated)
    {
        user.role = UserRole::Customer;
        user.customerSerial = customerSerial;
        return LoginResult::Authenticated;
    }
    if (customerResult == CustomerAuthentication::LoginNotConfigured)
        return LoginResult::CustomerLoginNotConfigured;
    if (customerResult == CustomerAuthentication::StorageError)
        return LoginResult::StorageError;
    return LoginResult::InvalidCredentials;
}

bool signIn(AuthenticatedUser &user)
{
    const int maximumAttempts = 4;
    for (int attempt = 1; attempt <= maximumAttempts; ++attempt)
    {
        heading("|| BANK LOGIN PAGE ||");
        cout << "Enter your administrator or customer ID; your role is detected automatically.\n"
             << "Type exit as the ID to close the program.\nID: ";
        string id;
        if (!getline(cin, id))
            return false;
        if (id.empty())
        {
            cout << "ID is required.\n";
            if (attempt < maximumAttempts)
                cout << maximumAttempts - attempt << " attempts left.\n";
            ps();
            continue;
        }
        string normalizedId = id;
        transform(normalizedId.begin(), normalizedId.end(), normalizedId.begin(),
                  [](unsigned char ch)
                  { return static_cast<char>(tolower(ch)); });
        if (normalizedId == "exit")
            return false;

        const string password = readPassword("Password: ");
        if (!cin)
            return false;

        const LoginResult result = authenticateUser(id, password, user);
        if (result == LoginResult::Authenticated)
        {
            footer(user.role == UserRole::Administrator
                       ? "ADMINISTRATOR LOGIN SUCCESSFUL"
                       : "CUSTOMER LOGIN SUCCESSFUL");
            ps();
            return true;
        }
        if (result == LoginResult::CustomerLoginNotConfigured)
            cout << "Customer login has not been set up. Ask the administrator to create it.\n";
        else if (result == LoginResult::StorageError)
        {
            cout << "Customer login storage could not be read. Contact the administrator.\n";
            ps();
            return false;
        }
        else
            cout << "ID or password is incorrect.\n";

        if (attempt < maximumAttempts)
            cout << maximumAttempts - attempt << " attempts left.\n";
        ps();
    }
    return false;
}

void showAdminProfile()
{
    pair<string, string> credentials = readCredentials();
    heading("|| ADMIN PROFILE ||");
    cout << "System       : Banking & Financial Management System\n"
         << "Administrator: " << credentials.first << '\n'
         << "Role         : System Administrator\n";
    ps();
}

void changeAdminPassword()
{
    pair<string, string> credentials = readCredentials();
    heading("|| CHANGE ADMIN PASSWORD ||");
    string currentPassword = readPassword("Current password: ");
    if (currentPassword != credentials.second)
    {
        cout << "Current password did not match.\n";
        ps();
        return;
    }

    string newPassword = readPassword("New password (at least 6 characters): ");
    if (newPassword.size() < 6)
    {
        cout << "Use at least 6 characters.\n";
        ps();
        return;
    }
    string confirmation = readPassword("Confirm new password: ");
    if (newPassword != confirmation)
    {
        cout << "The passwords did not match.\n";
        ps();
        return;
    }

    ofstream file("data/admin.txt", ios::trunc);
    if (!file.is_open())
        cout << "Unable to save the password. Check the data folder.\n";
    else
    {
        file << credentials.first << '|' << newPassword << '\n';
        file.flush();
        if (!file.good())
            cout << "Unable to save the password.\n";
        else
        {
            file.close();
            cout << (file.fail() ? "Unable to save the password.\n"
                                 : "Password updated successfully.\n");
        }
    }
    ps();
}
