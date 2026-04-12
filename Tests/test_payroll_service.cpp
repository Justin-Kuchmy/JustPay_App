#include <gtest/gtest.h>
#include "Services/PayrollService.h"
#include "Repositories/PayrollRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"

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

class PayrollDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<PayrollRepository> payrollRepo;
    std::unique_ptr<PayrollService> payrollService;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        payrollService.reset();

        payrollRepo = std::make_unique<PayrollRepository>(db);
        payrollService = std::make_unique<PayrollService>(*payrollRepo);
    }

    void TearDown() override
    {
        payrollService.reset();
        payrollRepo.reset();
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class PayrollServiceTest : public PayrollDbTest
{
protected:
    std::unique_ptr<PayrollService> service;

    void SetUp() override
    {
        PayrollDbTest::SetUp();
        service = std::make_unique<PayrollService>(*payrollRepo);
    }

    void TearDown() override
    {
        service.reset();
        PayrollDbTest::TearDown();
    }

    PayrollCalculationResults makeEmployee(double salary, double allowances = 0.0, bool sss = true, bool phic = true, bool hdmf = true, double otPay = 0.0, double loan = 0.0)
    {
        PayrollCalculationResults emp{};
        emp.monthlyBasicSalary = salary;
        emp.monthlyAllowances = allowances;
        emp.applySSS = sss;
        emp.applyPhilHealth = phic;
        emp.applyHDMF = hdmf;
        emp.overTimePay = otPay;
        emp.loanDeductionsPerPayroll = loan;
        emp.payPeriodHalf = 1;
        return emp;
    }
};

TEST_F(PayrollServiceTest, Applies_BothHalves_AlwaysTrue)
{
    int firstHalfOfMonth = 1;
    int secondHalfOfMonth = 2;
    EXPECT_TRUE(service->applies(DeductionSchedule::BothHalves, firstHalfOfMonth));
    EXPECT_TRUE(service->applies(DeductionSchedule::BothHalves, secondHalfOfMonth));
}

TEST_F(PayrollServiceTest, Applies_FirstHalf_OnlyMatchesHalfOne)
{
    int firstHalfOfMonth = 1;
    int secondHalfOfMonth = 2;
    EXPECT_TRUE(service->applies(DeductionSchedule::FirstHalf, firstHalfOfMonth));
    EXPECT_FALSE(service->applies(DeductionSchedule::FirstHalf, secondHalfOfMonth));
}

TEST_F(PayrollServiceTest, Applies_SecondHalf_OnlyMatchesHalfTwo)
{
    int firstHalfOfMonth = 1;
    int secondHalfOfMonth = 2;
    EXPECT_FALSE(service->applies(DeductionSchedule::SecondHalf, firstHalfOfMonth));
    EXPECT_TRUE(service->applies(DeductionSchedule::SecondHalf, secondHalfOfMonth));
}

// ---- TEST SSS ----
TEST_F(PayrollServiceTest, ComputeSSS)
{
    auto emp = makeEmployee(20000);
    double sss = service->computeSSS(emp);
    LOG_DEBUG("SSS: " << sss << " == " << std::min(emp.monthlyBasicSalary, 35000.0) * 0.05);
    EXPECT_DOUBLE_EQ(sss, std::min(emp.monthlyBasicSalary, 35000.0) * 0.05);
}

// ---- TEST PhilHealth ----
TEST_F(PayrollServiceTest, ComputePhilHealth)
{
    auto emp = makeEmployee(25000);
    double ph = service->computePhilHealth(emp);
    double cappedGross = std::clamp(emp.monthlyBasicSalary, 10000.0, 100000.0);
    EXPECT_DOUBLE_EQ(ph, cappedGross * 0.025);
}

// ---- TEST HDMF ----
TEST_F(PayrollServiceTest, ComputeHDMF)
{
    auto emp = makeEmployee(15000);
    double hdmf = service->computeHDMF(emp);
    double cappedGross = std::min(emp.monthlyBasicSalary, 10000.0);
    EXPECT_DOUBLE_EQ(hdmf, cappedGross * 0.02);
}

// ---- TEST Taxable Income ----
TEST_F(PayrollServiceTest, ComputeTaxableIncome)
{
    auto emp = makeEmployee(30000);
    emp.sssPremium_EE = service->computeSSS(emp);
    emp.philHealthPremium_EE = service->computePhilHealth(emp);
    emp.hdmfPremium_EE = service->computeHDMF(emp);

    double taxable = service->computeTaxableIncome(emp);
    EXPECT_DOUBLE_EQ(taxable, emp.monthlyBasicSalary + emp.overTimePay -
                                  emp.sssPremium_EE - emp.philHealthPremium_EE - emp.hdmfPremium_EE);
}

// ---- TEST Withholding ----
TEST_F(PayrollServiceTest, ComputeWithholding)
{
    double taxable = 20000.0;
    double tax = service->computeWithholding(taxable);

    // Use same calculation logic as SEMI_MONTHLY_TAX_BRACKETS for expected
    double expected = 0.0;
    for (size_t i = PayrollCalc::NUM_BRACKETS; i > 0; --i)
    {
        if (taxable < PayrollCalc::SEMI_MONTHLY_TAX_BRACKETS[i - 1].lowerBound)
            continue;
        expected = PayrollCalc::SEMI_MONTHLY_TAX_BRACKETS[i - 1].baseTax +
                   PayrollCalc::SEMI_MONTHLY_TAX_BRACKETS[i - 1].rate *
                       (taxable - PayrollCalc::SEMI_MONTHLY_TAX_BRACKETS[i - 1].lowerBound);
        break;
    }

    EXPECT_DOUBLE_EQ(tax, expected);
}

// ---- TEST Total Deductions ----
TEST_F(PayrollServiceTest, ComputeTotalDeductions)
{
    auto emp = makeEmployee(30000, 0.0, true, true, true, 0.0, 500.0);
    emp.sssPremium_EE = service->computeSSS(emp);
    emp.philHealthPremium_EE = service->computePhilHealth(emp);
    emp.hdmfPremium_EE = service->computeHDMF(emp);
    emp.withHoldingTax = service->computeWithholding(service->computeTaxableIncome(emp));

    double total = service->computeTotalDeductions(emp);
    double expected = emp.sssPremium_EE + emp.philHealthPremium_EE + emp.hdmfPremium_EE +
                      emp.withHoldingTax + emp.loanDeductionsPerPayroll;

    EXPECT_DOUBLE_EQ(total, expected);
}

// ---- TEST Net Pay ----
TEST_F(PayrollServiceTest, ComputeNetPay)
{
    auto emp = makeEmployee(30000, 0.0, true, true, true, 0.0, 500.0);
    emp.grossIncome = (emp.monthlyBasicSalary) + emp.overTimePay + (emp.monthlyAllowances);
    emp.sssPremium_EE = service->computeSSS(emp);
    emp.philHealthPremium_EE = service->computePhilHealth(emp);
    emp.hdmfPremium_EE = service->computeHDMF(emp);
    emp.withHoldingTax = service->computeWithholding(service->computeTaxableIncome(emp));
    emp.totalDeductions = service->computeTotalDeductions(emp);

    double net = service->computeNetPay(emp);
    double expected = emp.monthlyBasicSalary + emp.monthlyAllowances + emp.overTimePay - emp.totalDeductions;

    EXPECT_DOUBLE_EQ(net, expected);
}