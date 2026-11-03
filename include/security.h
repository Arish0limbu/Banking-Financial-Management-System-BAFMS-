#ifndef BAFMS_SECURITY_H
#define BAFMS_SECURITY_H

#include <string>

enum class UserRole
{
    Administrator,
    Customer
};

struct AuthenticatedUser
{
    UserRole role = UserRole::Administrator;
    int customerSerial = 0;
};

bool authenticateAdministrator(const std::string &id, const std::string &password);
enum class LoginResult
{
    Authenticated,
    InvalidCredentials,
    CustomerLoginNotConfigured,
    StorageError
};

LoginResult authenticateUser(const std::string &id, const std::string &password,
                             AuthenticatedUser &user);
bool signIn(AuthenticatedUser &user);
void showAdminProfile();
void changeAdminPassword();
std::string readPassword(const std::string &prompt);

#endif
