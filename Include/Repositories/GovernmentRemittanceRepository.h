#ifndef GOVERNMENT_REMITTANCE_REPOSITORY_H
#define GOVERNMENT_REMITTANCE_REPOSITORY_H
#include "Models/DataObjects.h"
#include "Repositories/BaseRepository.h"

class GovernmentRemittanceRepository : public BaseRepository
{
private:
    static GovernmentRemittance mapRemittance(sqlite3_stmt *stmt);

public:
    explicit GovernmentRemittanceRepository(sqlite3 *db);

    bool createTable() const override;

    sqlite3_int64 insertPayroll(const PayrollCalculationResults &payRoll);
};

#endif