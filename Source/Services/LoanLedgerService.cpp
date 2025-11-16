#include "Services/LoanLedgerService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"



LoanLedgerService::LoanLedgerService(LoanLedgerRepository& r): repo(r)
{

}

 //CREATE
int LoanLedgerService::addLoanLedger(const LoanLedger& LoanLedger)
{
    return this->repo.insertLoanLedger(LoanLedger);
}

//READ
std::optional<LoanLedger> LoanLedgerService::getLoanLedgerByID(int id)
{
    return  this->repo.getById(id);
}

std::vector<LoanLedger> LoanLedgerService::getAllLoanLedgers(std::string id)
{
    return this->repo.getAllById(id);
}

//UPDATE
bool LoanLedgerService::updateLoanLedger(const LoanLedger& e)
{
    return this->repo.updateLoanLedger(e);
}
        
//DELETE
bool LoanLedgerService::deleteLoanLedger(int id)
{
    return this->repo.deleteLoanLedger(id);
}