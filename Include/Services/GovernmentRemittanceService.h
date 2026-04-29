#ifndef GOVERNMENT_REMITTANCE_SERVICE_H
#define GOVERNMENT_REMITTANCE_SERVICE_H
#include "Repositories/GovernmentRemittanceRepository.h"
#include "Repositories/PayrollRepository.h"
#include <unordered_set>

class GovernmentRemittanceService
{
protected:
    GovernmentRemittanceRepository &remittanceRepo;
    PayrollRepository &payrollRepo;

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

    struct PeriodStatusSummary
    {
        std::string payPeriodDate;
        int payPeriodHalf;
        int sssRemittancesPending{0};
        int sssRemittancesSubmitted{0};
        int sssRemittancesConfirmed{0};
        int phicRemittancesPending{0};
        int phicRemittancesSubmitted{0};
        int phicRemittancesConfirmed{0};
        int hdmfRemittancesPending{0};
        int hdmfRemittancesSubmitted{0};
        int hdmfRemittancesConfirmed{0};
    };

    struct PeriodTotals
    {
        std::string payPeriodDate;
        int payPeriodHalf{1};
        int employeeCount{0};
        double totalSSSEE{0.0};
        double totalSSSER{0.0};
        double totalPHICEE{0.0};
        double totalPHICER{0.0};
        double totalHDMFEE{0.0};
        double totalHDMFER{0.0};
        double totalWithholdingTax{0.0};

        double totalEmployerCost() const
        {
            return totalSSSER + totalPHICER + totalHDMFER;
        }

        double totalEmployeeCost() const
        {
            return totalSSSEE + totalPHICEE + totalHDMFEE;
        }

        double grandTotal() const
        {
            return totalEmployerCost() + totalEmployeeCost() + totalWithholdingTax;
        }
    };
    explicit GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo);
    sqlite3_int64 addRemittance(const GovernmentRemittance &r);
    std::vector<sqlite3_int64> addRemittanceReports(const std::vector<GovernmentRemittance> &r);
    std::vector<GovernmentRemittance> createFromPayroll(const std::vector<PayrollCalculationResults> &payrolls);
    GovernmentRemittance createFromPayroll(const PayrollCalculationResults &payrolls, RemittanceType type);
    std::optional<GovernmentRemittance> getById(int id);
    std::optional<GovernmentRemittance> getByPayrollId(int payrollId);
    std::vector<GovernmentRemittance> getByPeriod(const std::string &payPeriodDate, std::optional<int> payPeriodHalf = std::nullopt);
    std::vector<GovernmentRemittance> getByEmployee(const std::string &employeeId);
    std::vector<GovernmentRemittance> getPending();
    std::vector<GovernmentRemittance> getAll();
    MonthlySummary getMonthlySummary(const std::string &monthYear);
    PeriodStatusSummary getPeriodStatusSummary(const std::string &payPeriodDate);
    PeriodTotals getPeriodTotals(const std::string &payPeriodDate, std::optional<int> payPeriodHalf = std::nullopt);
    double getTotalEmployerContribution(const std::vector<GovernmentRemittance> &remittances);
    double getTotalEmployeeContribution(const std::vector<GovernmentRemittance> &remittances);
    double getTotalRemittance(const std::vector<GovernmentRemittance> &remittances);
    bool submit(int remittanceId, const RemittanceType &remittanceType);
    bool confirm(int remittanceId, const RemittanceType &remittanceType);
    bool reject(int remittanceId, const RemittanceType &remittanceType);
    bool update(const GovernmentRemittance &remittance);
    bool remove(int id);
};

#endif