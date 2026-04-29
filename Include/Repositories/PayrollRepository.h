#ifndef PayRollEPO_H
#define PayRollEPO_H
#include "BaseRepository.h"
#include "Models/payroll.h"
#include <sqlite3.h>

class PayrollRepository : public BaseRepository
{
    static PayrollCalculationResults mapPayroll(sqlite3_stmt *stmt);
    static PayrollConfig mapPayrollConfig(sqlite3_stmt *stmt);
    static void bindPayroll(sqlite3_stmt *stmt, const PayrollCalculationResults &payroll);

public:
    explicit PayrollRepository(sqlite3 *db);

    bool createTable() const override;

    // create
    sqlite3_int64 insertPayroll(const PayrollCalculationResults &payRoll);

    // read
    std::vector<PayrollCalculationResults> getAll();
    std::optional<PayrollCalculationResults> getById(int id);
    std::vector<PayrollCalculationResults> getAllById(std::string &id);
    std::string getLatestPeriod();
    std::vector<PayrollCalculationResults> getPayrollByPeriod(const std::string &payPeriodDate, std::optional<std::string> employeeId = std::nullopt, std::optional<int> payPeriodHalf = std::nullopt);
    std::optional<PayrollConfig> loadConfig();
    bool saveConfig(const PayrollConfig &config);
    // update
    bool updatePayroll(const PayrollCalculationResults &lled);

    // delete
    bool deletePayroll(int id);
    int getLastPayrollId();
};

#endif