#ifndef LOANLEDGERSERVICE_H
#define LOANLEDGERSERVICE_H
#include "Repositories/LoanLedgerRepository.h"

class LoanLedgerService
{
    protected:
        LoanLedgerRepository& repo;
    public:
        explicit LoanLedgerService(LoanLedgerRepository& r);

        //CREATE
        int addLoanLedger(const LoanLedger& LoanLedger);

        //READ
        std::optional<LoanLedger> getLoanLedgerByID(int id);

        std::vector<LoanLedger> getAllLoanLedgers(std::string id);

        //UPDATE
        bool updateLoanLedger(const LoanLedger& e);
        
        //DELETE
        bool deleteLoanLedger(int id);
 
};

#endif