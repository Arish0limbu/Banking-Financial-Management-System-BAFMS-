#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#ifdef _WIN32
#include <conio.h>
#endif
#include "include/design.h"
#include "include/security.h"

using namespace std;

static pair<string, string> readCredentials()
{
    ifstream file("data/admin.txt");
    string id, password;
    if (file.is_open() && getline(file, id, '|') && getline(file, password))
        return {id, password};
    return {"admin", "123"};
}

static string readPassword()
{
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

bool adminlogin()
{
    const int maximumAttempts = 4;
    for (int attempt = 1; attempt <= maximumAttempts; ++attempt)
    {
        heading("|| ADMIN LOGIN PAGE ||");
        cout << "Enter ID: ";
        string id;
        if (!getline(cin, id))
            return false;
        cout << "Enter password: ";
        string password = readPassword();

        pair<string, string> credentials = readCredentials();
        if (id == credentials.first && password == credentials.second)
        {
            footer("LOGIN SUCCESSFUL");
            ps();
            return true;
        }
        footer("ID or password is incorrect.");
        if (attempt < maximumAttempts)
            centertxt(to_string(maximumAttempts - attempt) + " attempts left.");
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
    cout << "Current password: ";
    string currentPassword = readPassword();
    if (currentPassword != credentials.second)
    {
        cout << "Current password did not match.\n";
        ps();
        return;
    }

    cout << "New password (at least 6 characters): ";
    string newPassword = readPassword();
    if (newPassword.size() < 6)
    {
        cout << "Use at least 6 characters.\n";
        ps();
        return;
    }
    cout << "Confirm new password: ";
    string confirmation = readPassword();
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
        cout << (file.good() ? "Password updated successfully.\n" : "Unable to save the password.\n");
    }
    ps();
}
