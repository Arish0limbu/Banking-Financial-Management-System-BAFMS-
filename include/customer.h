#ifndef BAFMS_CUSTOMER_H
#define BAFMS_CUSTOMER_H

#include <string>

enum class CustomerAuthentication
{
    Authenticated,
    InvalidCredentials,
    LoginNotConfigured,
    StorageError
};

void customermanagement();
bool configureCustomerLogin();
bool setupCustomerLogin(int customerSerial);
CustomerAuthentication authenticateCustomer(const std::string &id,
                                            const std::string &password,
                                            int &customerSerial);
bool exportCustomerStatement(int customerSerial);
bool changeCustomerPassword(int customerSerial);
void customerPortal(int customerSerial);

#endif
