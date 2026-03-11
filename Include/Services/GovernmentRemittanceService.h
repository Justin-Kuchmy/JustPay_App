#ifndef GOVERNMENT_REMITTANCE_SERVICE_H
#define GOVERNMENT_REMITTANCE_SERVICE_H
#include "Repositories/GovernmentRemittanceRepository.h"
#include "Repositories/PayrollRepository.h"

// Get status summary for a period
struct PeriodStatusSummary
{
    std::string payPeriodText;
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
    std::string payPeriodText;
    int payPeriodHalf;
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

class GovernmentRemittanceService
{
protected:
    GovernmentRemittanceRepository &remittanceRepo;
    PayrollRepository &payrollRepo;

public:
    explicit GovernmentRemittanceService(GovernmentRemittanceRepository &remRepo, PayrollRepository &payRepo);

    std::vector<GovernmentRemittance> getRemittancesForWidget(const std::string &payPeriodText, int payPeriodHalf);

    bool generateRemittanceFromPayrolls(const std::vector<PayrollCalculationResults> *payrolls);

    std::vector<GovernmentRemittance> generateRemittancesForPeriod(const std::string &payPeriodText, int payPeriodHalf);

    bool submitRemittancesForPeriod(const std::string &payPeriodText, int payPeriodHalf, const std::string &remittanceType, int id);

    GovernmentRemittanceRepository::MonthlySummary getMonthlySummary(const std::string &monthYear);

    PeriodTotals calculatePeriodTotals(const std::string &payPeriodText, int payPeriodHalf);

    bool submitRemittance(int remittanceId, const std::string &remittanceType, int submittedByUserId);

    bool confirmRemittance(int remittanceId, const std::string &remittanceType);

    bool rejectRemittance(int remittanceId, const std::string &remittanceType);

    bool isPeriodFullySubmitted(const std::string &payPeriodText, int payPeriodHalf, const std::string &remittanceType);

    bool isPeriodFullyConfirmed(const std::string &payPeriodText, int payPeriodHalf, const std::string &remittanceType);

    PeriodStatusSummary getPeriodStatusSummary(const std::string &payPeriodText, int payPeriodHalf);
};

#endif