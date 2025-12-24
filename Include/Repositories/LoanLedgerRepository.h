#ifndef LOANLEDGEREPO_H
#define LOANLEDGEREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class LoanLedgerRepository: public BaseRepository
{
    static LoanLedger mapLoanLedger(sqlite3_stmt* stmt);
    public: 
        explicit LoanLedgerRepository(sqlite3* db);

        std::string getCreateTableSQL() const override;

        //create
        sqlite3_int64 insertLoanLedger(const LoanLedger& loanLedger);
        
        //read
        std::optional<LoanLedger> getById(int id);
        std::vector<LoanLedger> getAllById(std::string id);
        
        //update
        bool updateLoanLedger(const LoanLedger& lled);
        
        //delete
        bool deleteLoanLedger(int id);
        std::string getLastLoanLedgerId();
};


#endif