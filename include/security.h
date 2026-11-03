#ifndef BAFMS_SECURITY_H
#define BAFMS_SECURITY_H

#include <string>

bool adminlogin();
bool authenticateAdministrator(const std::string &id, const std::string &password);
void showAdminProfile();
void changeAdminPassword();
std::string readPassword(const std::string &prompt);

#endif
