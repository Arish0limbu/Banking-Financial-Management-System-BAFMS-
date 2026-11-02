#ifndef BAFMS_CUSTOMER_H
#define BAFMS_CUSTOMER_H

void customermanagement();
bool configureCustomerLogin();
bool setupCustomerLogin(int customerSerial);
bool customerlogin(int &customerSerial);
void customerPortal(int customerSerial);

#endif
