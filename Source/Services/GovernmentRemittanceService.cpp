#include "Services/GovernmentRemittanceService.h"

GovernmentRemittanceService::GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo) : remittanceRepo(remRepo), payrollRepo(payRepo)
{
}

sqlite3_int64 GovernmentRemittanceService::addPayroll(const PayrollCalculationResults &payrollCalculationResults)
{
    return {};
}
