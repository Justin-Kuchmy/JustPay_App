#ifndef PayRollEPO_H
#define PayRollEPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class PayrollRepository : public BaseRepository
{
    static PayrollCalculationResults mapPayroll(sqlite3_stmt *stmt);

public:
    explicit PayrollRepository(sqlite3 *db);

    bool createTable() const override;

    // create
    sqlite3_int64 insertPayroll(const PayrollCalculationResults &payRoll);

    // read
    std::optional<PayrollCalculationResults> getById(int id);
    std::vector<PayrollCalculationResults> getAllById(std::string &id);
    std::optional<PayrollCalculationResults> getPayrollByEmployeeAndPeriod(const std::string &employeeId, const std::string &payPeriodText, int payPeriodHalf);

    // update
    bool updatePayroll(const PayrollCalculationResults &lled);

    // delete
    bool deletePayroll(int id);
    std::string getLastPayrollId();
};

#endif