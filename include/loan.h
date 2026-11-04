#ifndef BAFMS_LOAN_H
#define BAFMS_LOAN_H

#include <string>
#include <vector>

constexpr double STANDARD_LOAN_ANNUAL_RATE = 10.0;

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

struct LoanEstimate
{
    double principal;
    double interest;
    double totalRepayment;
    double estimatedMonthlyPayment;
};

std::vector<LoanRecord> readLoans();
bool saveLoans(const std::vector<LoanRecord> &loans);
double loanOutstandingBalance(const LoanRecord &loan);
bool estimateSimpleInterestLoan(double principal, int termMonths, LoanEstimate &estimate);
void estimateCustomerLoan();
void applyForCustomerLoan(int customerSerial);
void loanmanagement();

#endif
