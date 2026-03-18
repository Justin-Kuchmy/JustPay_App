

#ifndef GOVERNMENT_REMITTANCE_REPOSITORY_H
#define GOVERNMENT_REMITTANCE_REPOSITORY_H
#include "Models/DataObjects.h"
#include "Repositories/BaseRepository.h"
#include <sqlite3.h>
#include <vector>
#include <optional>

class GovernmentRemittanceRepository : public BaseRepository
{
private:
    static GovernmentRemittance mapRemittance(sqlite3_stmt *stmt);
    static int bindRemittance(sqlite3_stmt *stmt, const GovernmentRemittance &remittance);

public:
    struct MonthlySummary
    {
        std::string monthYear;
        double totalSSSEE{0.0};
        double totalSSSER{0.0};
        double totalPHICEE{0.0};
        double totalPHICER{0.0};
        double totalHDMFEE{0.0};
        double totalHDMFER{0.0};
        double totalWithholdingTax{0.0};
    };

    explicit GovernmentRemittanceRepository(sqlite3 *db);

    bool createTable() const override;

    // create
    sqlite3_int64 insertRemittance(const GovernmentRemittance &r);
    std::vector<sqlite3_int64> insertRemittanceReports(const std::vector<GovernmentRemittance> &r);

    // read
    std::optional<GovernmentRemittance> getRemittanceById(int id);
    std::optional<GovernmentRemittance> getRemittanceByPayrollId(int payrollId);
    std::vector<GovernmentRemittance> getRemittancesByPeriod(const std::string &payPeriodText, std::optional<int> payPeriodHalf = std::nullopt);
    std::vector<GovernmentRemittance> getRemittancesByEmployee(const std::string &employeeId);
    std::vector<GovernmentRemittance> getRemittancesByStatus(RemittanceStatus status);
    std::vector<GovernmentRemittance> getAllRemittances();
    MonthlySummary getMonthlySummary(const std::string &monthYear);

    // update -
    bool markAsSubmitted(int remittanceId, const RemittanceType &remittanceType);
    bool markAsConfirmed(int remittanceId, const RemittanceType &remittanceType);
    bool markAsRejected(int remittanceId, const RemittanceType &remittanceType);
    bool updateRemittance(const GovernmentRemittance &remittance);
    // delete
    bool deleteRemittance(int id);
};

#endif