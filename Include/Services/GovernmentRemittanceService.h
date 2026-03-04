#ifndef GOVERNMENT_REMITTANCE_SERVICE_H
#define GOVERNMENT_REMITTANCE_SERVICE_H
#include "Repositories/GovernmentRemittanceRepository.h"
#include "Repositories/PayrollRepository.h"

class GovernmentRemittanceService
{
protected:
    GovernmentRemittanceRepository &remittanceRepo;
    PayrollRepository &payrollRepo;

public:
    explicit GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo);

    sqlite3_int64 addPayroll(const PayrollCalculationResults &payrollCalculationResults);
};

#endif