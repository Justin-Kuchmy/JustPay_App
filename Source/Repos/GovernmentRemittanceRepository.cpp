#include "Repositories/GovernmentRemittanceRepository.h"

GovernmentRemittanceRepository::GovernmentRemittanceRepository(sqlite3 *db) : BaseRepository(db)
{
}

GovernmentRemittance GovernmentRemittanceRepository::mapRemittance(sqlite3_stmt *stmt)
{
    return {};
}

bool GovernmentRemittanceRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/payroll.sql");
}

sqlite3_int64 GovernmentRemittanceRepository::insertPayroll(const PayrollCalculationResults &payRoll)
{
    return {};
}
