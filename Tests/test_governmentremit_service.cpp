#include <gtest/gtest.h>
#include "Services/GovernmentRemittanceService.h"
#include "Repositories/GovernmentRemittanceRepository.h"
#include "Repositories/PayrollRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Helpers/PayrollCalculations.h"
#include <algorithm>
#include <ranges>
using namespace PayrollCalc;

static void runSqlScript(sqlite3 *db, const std::string &sqlFilePath)
{
    std::ifstream file(sqlFilePath);
    ASSERT_TRUE(file.is_open()) << "Failed to open SQL file: " << sqlFilePath;
    std::string sql((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    char *errMsg = nullptr;
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQLite error running script: " << sqlFilePath << "\n";
        std::cerr << "   → Error message: " << (errMsg ? errMsg : "unknown") << "\n";

        std::cerr << "   → Script snippet near failure:\n";
        size_t pos = sql.find_last_of(';');
        if (pos != std::string::npos)
        {
            std::string snippet = sql.substr(pos > 200 ? pos - 200 : 0, 200);
            std::cerr << snippet << "\n";
        }

        ASSERT_EQ(rc, SQLITE_OK) << "SQL error: " << (errMsg ? errMsg : "unknown");
    }
    if (errMsg)
        sqlite3_free(errMsg);
};

class GovRemitDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<GovernmentRemittanceRepository> remitRepo;
    std::unique_ptr<PayrollRepository> payrollRepo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        remitRepo = std::make_unique<GovernmentRemittanceRepository>(db);
        payrollRepo = std::make_unique<PayrollRepository>(db);
    }

    void TearDown() override
    {
        remitRepo.reset();
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

struct Contributions
{
    bool sss = false;
    bool phic = false;
    bool hdmf = false;
};

class GovRemitServiceTest : public GovRemitDbTest
{
protected:
    std::unique_ptr<GovernmentRemittanceService> service;

    void SetUp() override
    {
        GovRemitDbTest::SetUp();
        service = std::make_unique<GovernmentRemittanceService>(*remitRepo, *payrollRepo);
    }

    void TearDown() override
    {
        service.reset();
        GovRemitDbTest::TearDown();
    }
};

PayrollCalculationResults makeEmployeePayroll(double salary, std::string payPeriodDate, double allowances = 0.0, Contributions type = {true, false, false}, double otPay = 0.0, double loan = 0.0, int payPeriodHalf = 1)
{
    PayrollCalculationResults emp{};
    emp.id = 1;
    emp.employeeId = "00-0001";
    emp.monthlyBasicSalary = salary;
    emp.monthlyAllowances = allowances;
    emp.applySSS = type.sss;
    emp.applyPhilHealth = type.phic;
    emp.applyHDMF = type.hdmf;
    emp.overTimePay = otPay;
    emp.loanDeductionsPerPayroll = loan;
    emp.payPeriodHalf = payPeriodHalf;
    emp.payPeriodDate = payPeriodDate;
    emp.sssPremium_EE = PayrollCalc::calcSSS_EE(emp.monthlyBasicSalary, emp.applySSS);
    emp.sssPremium_ER = PayrollCalc::calcSSS_ER(emp.monthlyBasicSalary, emp.applySSS);

    emp.philHealthPremium_EE = PayrollCalc::calcPhilHealth_EE(emp.monthlyBasicSalary, emp.applyPhilHealth);
    emp.philHealthPremium_ER = PayrollCalc::calcPhilHealth_ER(emp.monthlyBasicSalary, emp.applyPhilHealth);

    emp.hdmfPremium_EE = PayrollCalc::calcHDMF_EE(emp.monthlyBasicSalary, emp.applyHDMF);
    emp.hdmfPremium_ER = PayrollCalc::calcHDMF_ER(emp.monthlyBasicSalary, emp.applyHDMF);

    return emp;
};

TEST_F(GovRemitServiceTest, createFromPayroll_vector_returnsRemittancesForEachPayroll)
{
    // std::vector<GovernmentRemittance> createFromPayroll(const std::vector<PayrollCalculationResults> &payrolls);

    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo};
    auto result = service->createFromPayroll(payrolls);
    EXPECT_TRUE(payrolls.size() == result.size());
}
TEST_F(GovRemitServiceTest, createFromPayroll_vector_returnsEmptyWhenPayrollsEmpty)
{
    // std::vector<GovernmentRemittance> createFromPayroll(const std::vector<PayrollCalculationResults> &payrolls);
    std::vector<PayrollCalculationResults> payrolls{};
    auto result = service->createFromPayroll(payrolls);
    EXPECT_TRUE(result.empty());
}

TEST_F(GovRemitServiceTest, createFromPayroll_single_returnsCorrectRemittanceForType)
{
    // std::vector<GovernmentRemittance> createFromPayroll(const std::vector<PayrollCalculationResults> &payrolls);

    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = false, .phic = true, .hdmf = false}, 0.00, 0.00, 2);
    PayrollCalculationResults payrollThree = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = true}, 0.00, 0.00, 2);
    GovernmentRemittance resultA = service->createFromPayroll(payrollOne, RemittanceType::SSS);
    GovernmentRemittance resultB = service->createFromPayroll(payrollTwo, RemittanceType::PHIC);
    GovernmentRemittance resultC = service->createFromPayroll(payrollThree, RemittanceType::HDMF);

    EXPECT_EQ(resultA.contrib_Type, RemittanceType::SSS);
    EXPECT_EQ(resultB.contrib_Type, RemittanceType::PHIC);
    EXPECT_EQ(resultC.contrib_Type, RemittanceType::HDMF);

    EXPECT_EQ(resultA.total_Contrib, (payrollOne.sssPremium_EE + payrollOne.sssPremium_ER));
    EXPECT_EQ(resultB.total_Contrib, (payrollTwo.philHealthPremium_EE + payrollTwo.philHealthPremium_ER));
    EXPECT_EQ(resultC.total_Contrib, (payrollThree.hdmfPremium_EE + payrollThree.hdmfPremium_ER));
}

TEST_F(GovRemitServiceTest, addRemittanceReports_returnsIdsForValidReports)
{
    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = false, .phic = true, .hdmf = true}, 0.00, 0.00, 2);
    PayrollCalculationResults payrollThree = makeEmployeePayroll(22500, "June 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 1000.00, 500.00, 1);
    PayrollCalculationResults payrollFour = makeEmployeePayroll(22500, "June 2025", 2500, Contributions{.sss = false, .phic = true, .hdmf = true}, 500.00, 500.00, 2);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo, payrollThree, payrollFour};
    auto remits = service->createFromPayroll(payrolls);
    std::vector<sqlite3_int64> ids = service->addRemittanceReports(remits);

    for (auto &id : ids)
    {
        std::optional<GovernmentRemittance> returnedRemit = service->getById(id);
        if (returnedRemit.has_value())
        {
            auto valid = returnedRemit.value();
            LOG_DEBUG(id << " " << valid.id);
            EXPECT_EQ(valid.id, id);
        }
    }
}

TEST_F(GovRemitServiceTest, getMonthlySummary_returnsCorrectSummaryForValidMonth)
{
    // MonthlySummary getMonthlySummary(const std::string &monthYear);

    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = false, .phic = true, .hdmf = true}, 0.00, 0.00, 2);
    PayrollCalculationResults payrollThree = makeEmployeePayroll(22500, "June 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 1000.00, 500.00, 1);
    PayrollCalculationResults payrollFour = makeEmployeePayroll(22500, "June 2025", 2500, Contributions{.sss = false, .phic = true, .hdmf = true}, 500.00, 500.00, 2);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo, payrollThree, payrollFour};
    auto remits = service->createFromPayroll(payrolls);
    service->addRemittanceReports(remits);

    std::string mthYearToFind{"May 2025"};
    GovernmentRemittanceService::MonthlySummary result = service->getMonthlySummary(mthYearToFind);
    GovernmentRemittanceService::MonthlySummary expected{};

    std::erase_if(remits, [&mthYearToFind](const GovernmentRemittance &item)
                  { return item.payPeriodDate != mthYearToFind; });

    expected.monthYear = mthYearToFind;
    for (GovernmentRemittance &item : remits)
    {
        switch (static_cast<int>(item.contrib_Type))
        {
        case 0:
            expected.totalSSSEE += item.employee_Contrib;
            expected.totalSSSER += item.employer_Contrib;
            break;
        case 1:
            expected.totalPHICEE += item.employee_Contrib;
            expected.totalPHICER += item.employer_Contrib;
            break;
        case 2:
            expected.totalHDMFEE += item.employee_Contrib;
            expected.totalHDMFER += item.employer_Contrib;
            break;
        }
        expected.totalWithholdingTax += item.withHoldingTax;
    }

    EXPECT_EQ(result.monthYear, expected.monthYear);
    EXPECT_DOUBLE_EQ(result.totalSSSEE, expected.totalSSSEE);
    EXPECT_DOUBLE_EQ(result.totalSSSER, expected.totalSSSER);
    EXPECT_DOUBLE_EQ(result.totalPHICEE, expected.totalPHICEE);
    EXPECT_DOUBLE_EQ(result.totalPHICER, expected.totalPHICER);
    EXPECT_DOUBLE_EQ(result.totalHDMFEE, expected.totalHDMFEE);
    EXPECT_DOUBLE_EQ(result.totalHDMFER, expected.totalHDMFER);
    EXPECT_DOUBLE_EQ(result.totalWithholdingTax, expected.totalWithholdingTax);
}
TEST_F(GovRemitServiceTest, getMonthlySummary_returnsEmptySummaryForMonthWithNoData)
{
    // MonthlySummary getMonthlySummary(const std::string &monthYear);
    GovernmentRemittanceService::MonthlySummary result = service->getMonthlySummary("June 2025");
    EXPECT_EQ(result.totalSSSEE, 0.0);
    EXPECT_EQ(result.totalSSSER, 0.0);
    EXPECT_EQ(result.totalPHICEE, 0.0);
    EXPECT_EQ(result.totalPHICER, 0.0);
    EXPECT_EQ(result.totalHDMFEE, 0.0);
    EXPECT_EQ(result.totalHDMFER, 0.0);
    EXPECT_EQ(result.totalWithholdingTax, 0.0);
}
TEST_F(GovRemitServiceTest, getMonthlySummary_handlesInvalidMonthYearFormat)
{
    // MonthlySummary getMonthlySummary(const std::string &monthYear);

    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 2500, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = false, .phic = true, .hdmf = true}, 0.00, 0.00, 2);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo};
    auto remits = service->createFromPayroll(payrolls);
    service->addRemittanceReports(remits);

    // doesnt crash or cause an error, just returns an empty object,
    GovernmentRemittanceService::MonthlySummary result = service->getMonthlySummary("not valid");
    EXPECT_EQ(result.totalSSSEE, 0.0);
    EXPECT_EQ(result.totalSSSER, 0.0);
    EXPECT_EQ(result.totalPHICEE, 0.0);
    EXPECT_EQ(result.totalPHICER, 0.0);
    EXPECT_EQ(result.totalHDMFEE, 0.0);
    EXPECT_EQ(result.totalHDMFER, 0.0);
    EXPECT_EQ(result.totalWithholdingTax, 0.0);
}

TEST_F(GovRemitServiceTest, getPeriodTotals_returnsCorrectTotalsForFirstHalf)
{
    // PeriodTotals getPeriodTotals(const std::string &payPeriodDate, int payPeriodHalf);

    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(32500, "May 2025", 2500.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 1235.00, 575.00, 1);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo};
    auto remits = service->createFromPayroll(payrolls);
    service->addRemittanceReports(remits);

    auto sssOneEE = payrollOne.sssPremium_EE;
    auto sssOneER = payrollOne.sssPremium_ER;
    auto sssTwoEE = payrollTwo.sssPremium_EE;
    auto sssTwoER = payrollTwo.sssPremium_ER;
    auto exptectedSSSEE = sssOneEE + sssTwoEE;
    auto exptectedSSSER = sssOneER + sssTwoER;

    GovernmentRemittanceService::PeriodTotals totals = service->getPeriodTotals("May 2025", 1);
    EXPECT_DOUBLE_EQ(exptectedSSSEE, totals.totalSSSEE);
    EXPECT_DOUBLE_EQ(exptectedSSSER, totals.totalSSSER);
}

TEST_F(GovRemitServiceTest, getPeriodTotals_returnsCorrectTotalsForSecondHalf)
{
    // PeriodTotals getPeriodTotals(const std::string &payPeriodDate, int payPeriodHalf);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(18000.00, "May 2025", 2500.00, Contributions{.sss = false, .phic = true, .hdmf = true}, 900.00, 575.00, 2);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo};
    auto remits = service->createFromPayroll(payrolls);
    service->addRemittanceReports(remits);

    auto sssOneEE = payrollOne.sssPremium_EE;
    auto sssOneER = payrollOne.sssPremium_ER;
    auto sssTwoEE = payrollTwo.sssPremium_EE;
    auto sssTwoER = payrollTwo.sssPremium_ER;
    auto exptectedSSSEE = sssOneEE + sssTwoEE;
    auto exptectedSSSER = sssOneER + sssTwoER;

    GovernmentRemittanceService::PeriodTotals totals = service->getPeriodTotals("May 2025", 1);
    EXPECT_DOUBLE_EQ(exptectedSSSEE, totals.totalSSSEE);
    EXPECT_DOUBLE_EQ(exptectedSSSER, totals.totalSSSER);
}
TEST_F(GovRemitServiceTest, getPeriodTotals_returnsZeroTotalsForPeriodWithNoData)
{
    // PeriodTotals getPeriodTotals(const std::string &payPeriodDate, int payPeriodHalf);
    GovernmentRemittanceService::PeriodTotals totals = service->getPeriodTotals("December 2099", 1);

    EXPECT_EQ(totals.employeeCount, 0);
    EXPECT_DOUBLE_EQ(totals.totalSSSEE, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalSSSER, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalPHICEE, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalPHICER, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalHDMFEE, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalHDMFER, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalWithholdingTax, 0.0);
}
TEST_F(GovRemitServiceTest, getPeriodTotals_handlesInvalidPayPeriodHalf)
{
    // PeriodTotals getPeriodTotals(const std::string &payPeriodDate, int payPeriodHalf);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(22500, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    std::vector<PayrollCalculationResults> payrolls{payrollOne};
    auto remits = service->createFromPayroll(payrolls);
    service->addRemittanceReports(remits);

    // payPeriodHalf 99 matches nothing — expect zeros, no crash
    GovernmentRemittanceService::PeriodTotals totals = service->getPeriodTotals("May 2025", 99);

    EXPECT_EQ(totals.employeeCount, 0);
    EXPECT_DOUBLE_EQ(totals.totalSSSEE, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalSSSER, 0.0);
}
TEST_F(GovRemitServiceTest, getPeriodTotals_handlesInvalidPeriodText)
{
    // PeriodTotals getPeriodTotals(const std::string &payPeriodDate, int payPeriodHalf);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(14000, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    std::vector<PayrollCalculationResults> payrolls{payrollOne};
    auto remits = service->createFromPayroll(payrolls);
    service->addRemittanceReports(remits);

    GovernmentRemittanceService::PeriodTotals totals = service->getPeriodTotals("NOT-A-PERIOD", 1);

    EXPECT_EQ(totals.employeeCount, 0);
    EXPECT_DOUBLE_EQ(totals.totalSSSEE, 0.0);
    EXPECT_DOUBLE_EQ(totals.totalSSSER, 0.0);
}
TEST_F(GovRemitServiceTest, getTotalEmployerContribution_returnsSumOfAllEmployerContributions)
{
    // double getTotalEmployerContribution(const std::vector<GovernmentRemittance> &remittances);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(14000.00, "May 2025", 2500.00, Contributions{.sss = false, .phic = true, .hdmf = true}, 900.00, 575.00, 2);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo};
    auto remits = service->createFromPayroll(payrolls);
    auto expectedSum{1113.0 + 350.0 + 200.0};
    // Employer Contribution calculate sss, phic, hdmf
    // std::min(14000,static_cast<double>(35000)) *0.0795; = 1113.0
    // std::clamp(14000,static_cast<double>(10000),static_cast<double>(100000)) *0.025; =  350.0
    // std::min(14000,static_cast<double>(10000)) *0.02; = 200

    EXPECT_EQ(expectedSum, service->getTotalEmployerContribution(remits));
}
TEST_F(GovRemitServiceTest, getTotalEmployerContribution_returnsZeroForEmptyRemittances)
{
    // double getTotalEmployerContribution(const std::vector<GovernmentRemittance> &remittances);
    std::vector<PayrollCalculationResults> payrolls{};
    auto remits = service->createFromPayroll(payrolls);
    auto expectedSum{0.0};
    EXPECT_EQ(expectedSum, service->getTotalEmployerContribution(remits));
}
TEST_F(GovRemitServiceTest, getTotalEmployerContribution_returnsSingleValueWhenOneRemittance)
{
    // double getTotalEmployerContribution(const std::vector<GovernmentRemittance> &remittances);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    std::vector<PayrollCalculationResults> payrolls{payrollOne};
    auto remits = service->createFromPayroll(payrolls);
    auto expectedSum{1113.0};
    // Employer Contribution calculate sss
    // std::min(14000,static_cast<double>(35000)) *0.0795; = 1113
    EXPECT_EQ(expectedSum, service->getTotalEmployerContribution(remits));
}
TEST_F(GovRemitServiceTest, getTotalEmployeeContribution_returnsSumOfAllEmployeeContributions)
{
    // double getTotalEmployeeContribution(const std::vector<GovernmentRemittance> &remittances);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    PayrollCalculationResults payrollTwo = makeEmployeePayroll(14000.00, "May 2025", 2500.00, Contributions{.sss = false, .phic = true, .hdmf = true}, 900.00, 575.00, 2);
    std::vector<PayrollCalculationResults> payrolls{payrollOne, payrollTwo};
    auto remits = service->createFromPayroll(payrolls);
    auto expectedSum{700.0 + 350.0 + 200.0};
    // Employee Contribution: calculate sss, phic, hdmf
    // std::min(14000,static_cast<double>(35000)) *  0.05; =  700
    // std::clamp(14000,static_cast<double>(10000),static_cast<double>(100000)) *0.025; = 350.0
    // std::min(14000,static_cast<double>(10000)) *0.02; = 200
    EXPECT_EQ(expectedSum, service->getTotalEmployeeContribution(remits));
}
TEST_F(GovRemitServiceTest, getTotalEmployeeContribution_returnsZeroForEmptyRemittances)
{
    // double getTotalEmployeeContribution(const std::vector<GovernmentRemittance> &remittances);
    std::vector<PayrollCalculationResults> payrolls;
    auto remits = service->createFromPayroll(payrolls);
    auto expectedSum{0.0};
    EXPECT_EQ(expectedSum, service->getTotalEmployeeContribution(remits));
}
TEST_F(GovRemitServiceTest, getTotalEmployeeContribution_returnsSingleValueWhenOneRemittance)
{
    // double getTotalEmployeeContribution(const std::vector<GovernmentRemittance> &remittances);
    PayrollCalculationResults payrollOne = makeEmployeePayroll(14000.00, "May 2025", 1000.00, Contributions{.sss = true, .phic = false, .hdmf = false}, 0.00, 0.00, 1);
    std::vector<PayrollCalculationResults> payrolls{payrollOne};
    auto remits = service->createFromPayroll(payrolls);
    auto expectedSum{700.0};
    // Employee Contribution: calculate sss
    // std::min(14000,static_cast<double>(35000)) *  0.05; = 700.0
    EXPECT_EQ(expectedSum, service->getTotalEmployeeContribution(remits));
}