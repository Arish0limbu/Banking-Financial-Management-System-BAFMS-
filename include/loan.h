#ifndef BAFMS_LOAN_H
#define BAFMS_LOAN_H

#include <string>
#include <vector>

struct LoanRecord
{
    int loanId;
    int customerSerial;
    double principal;
    double annualRate;
    int termMonths;
    double amountPaid;
    std::string status;
};

std::vector<LoanRecord> readLoans();
bool saveLoans(const std::vector<LoanRecord> &loans);
void loanmanagement();

#endif
