#include <gtest/gtest.h>
#include "Services/YearEndBenefitsService.h"
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

class BaseDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<PayrollRepository> payrollRepo;
    std::unique_ptr<LeaveRepository> leaveRepo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        payrollRepo = std::make_unique<PayrollRepository>(db);
        leaveRepo = std::make_unique<LeaveRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class YearEndBenefitsServiceTest : public BaseDbTest
{
protected:
    std::unique_ptr<YearEndBenefitsService> svc;

    void SetUp() override
    {
        BaseDbTest::SetUp();
        svc = std::make_unique<YearEndBenefitsService>(*payrollRepo, *leaveRepo);
    }

    void TearDown() override
    {
    }
};

TEST_F(YearEndBenefitsServiceTest, Computes13thMonthCorrectly)
{
    std::string empId = "00-0001";

    auto result = svc->compute(empId, 2025);

    double expected13th = 0.0;

    auto payrolls = payrollRepo->getAllById(empId);

    for (const auto &p : payrolls)
    {
        size_t pos = p.payPeriodText.find(' ');
        if (pos == std::string::npos)
            continue;

        int year = std::stoi(p.payPeriodText.substr(pos + 1));
        if (year == 2025)
        {
            expected13th += (p.monthlyBasicSalary / 12.0);
        }
    }

    EXPECT_DOUBLE_EQ(result.thirteenthMonthPay, expected13th);
}

TEST_F(YearEndBenefitsServiceTest, IgnoresOtherYears)
{
    std::string empId = "00-0001";

    auto result2025 = svc->compute(empId, 2025);
    auto result2024 = svc->compute(empId, 2024);

    EXPECT_NE(result2025.totalBasicSalary, result2024.totalBasicSalary);
}

TEST_F(YearEndBenefitsServiceTest, ReturnsZeroWhenNoPayroll)
{
    std::string empId = "NON_EXISTENT";

    auto result = svc->compute(empId, 2025);

    EXPECT_DOUBLE_EQ(result.totalBasicSalary, 0.0);
    EXPECT_DOUBLE_EQ(result.thirteenthMonthPay, 0.0);
    EXPECT_DOUBLE_EQ(result.monetizedLeaveValue, 0.0);
}

TEST_F(YearEndBenefitsServiceTest, HandlesMultiplePayrollEntries)
{
    std::string empId = "00-0001";

    auto result = svc->compute(empId, 2025);

    auto payrolls = payrollRepo->getAllById(empId);

    int count2025 = 0;
    for (const auto &p : payrolls)
    {
        size_t pos = p.payPeriodText.find(' ');
        if (pos == std::string::npos)
            continue;

        int year = std::stoi(p.payPeriodText.substr(pos + 1));
        if (year == 2025)
            count2025++;
    }

    EXPECT_GT(count2025, 1);

    EXPECT_GT(result.totalBasicSalary, 0.0);
    EXPECT_GT(result.thirteenthMonthPay, 0.0);
}

TEST_F(YearEndBenefitsServiceTest, ComputesUnusedLeaveDaysCorrectly)
{
    EXPECT_EQ(1, 0);
}

TEST_F(YearEndBenefitsServiceTest, ComputesDailyRateCorrectly)
{
    EXPECT_EQ(1, 0);
}

TEST_F(YearEndBenefitsServiceTest, ComputesMonetizedLeaveValueCorrectly)
{
    EXPECT_EQ(1, 0);
}
