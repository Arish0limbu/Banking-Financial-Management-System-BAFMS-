#ifndef BAFMS_CUSTOMER_H
#define BAFMS_CUSTOMER_H

#include <map>
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
bool customerExists(int customerSerial);
std::map<int, std::string> customerNamesBySerial();
CustomerAuthentication authenticateCustomer(const std::string &id,
                                            const std::string &password,
                                            int &customerSerial);
bool exportCustomerStatement(int customerSerial);
bool exportFilteredCustomerStatement(int customerSerial);
bool exportCustomerRecordsCsv();
bool changeCustomerPassword(int customerSerial);
void customerPortal(int customerSerial);

#endif
