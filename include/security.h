#ifndef BAFMS_SECURITY_H
#define BAFMS_SECURITY_H

#include <string>

bool adminlogin();
void showAdminProfile();
void changeAdminPassword();
std::string readPassword(const std::string &prompt);

#endif
