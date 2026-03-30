#include <gtest/gtest.h>
#include "Services/YearEndBenefitsService.h"
#include "Services/LeaveBalanceService.h"
#include "Repositories/EmployeeRepository.h"
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

class YearEndDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<PayrollRepository> payrollRepo;
    std::unique_ptr<LeaveRepository> leaveRepo;
    std::unique_ptr<EmployeeRepository> empRepo;
    std::unique_ptr<LeaveBalanceService> leaveBalanceService;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        leaveBalanceService.reset();

        payrollRepo = std::make_unique<PayrollRepository>(db);
        leaveRepo = std::make_unique<LeaveRepository>(db);
        empRepo = std::make_unique<EmployeeRepository>(db);
        leaveBalanceService = std::make_unique<LeaveBalanceService>(*leaveRepo, *empRepo);
    }

    void TearDown() override
    {
        leaveBalanceService.reset();
        payrollRepo.reset();
        leaveRepo.reset();
        empRepo.reset();
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class YearEndBenefitsServiceTest : public YearEndDbTest
{
protected:
    std::unique_ptr<YearEndBenefitsService> svc;

    void SetUp() override
    {
        YearEndDbTest::SetUp();
        svc = std::make_unique<YearEndBenefitsService>(*payrollRepo, *leaveBalanceService, *empRepo);
    }

    void TearDown() override
    {
        svc.reset();
        YearEndDbTest::TearDown();
    }
};

TEST_F(YearEndBenefitsServiceTest, Computes13thMonthCorrectly)
{
    std::string empId = "00-0001";

    auto result = svc->compute(empId, 2025);
    LOG_DEBUG(result.to_string());

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

static Employee makeEmployee(const std::string &fullName, const std::string &dateHired, double salary = 20000.0)
{
    Employee emp{};
    emp.fullName = fullName;
    emp.department = Department(0);
    emp.position = "HR Manager";
    emp.jobLevel = JobLevel(2);
    emp.status = EmploymentStatus(0);
    emp.dateHired = Date::fromString(dateHired);
    emp.monthlyBasicSalary = salary;
    emp.isActive = true;
    return emp;
}

static EmployeeLeaveBalance makeLeaveBalance(const std::string &employeeId, int year, double earned, double used)
{
    EmployeeLeaveBalance bal{};
    bal.employeeId = employeeId;
    bal.year = year;
    bal.totalLeaveEarned = earned;
    bal.leaveUsed = used;
    bal.unusedLeaveDays = earned - used;
    return bal;
}

TEST_F(YearEndBenefitsServiceTest, ComputesUnusedLeaveDaysCorrectly)
{

    std::string empId = empRepo->insertEmployee(makeEmployee("Bob Reyes", "2022-01-15", 45000.0));
    leaveRepo->insert(makeLeaveBalance(empId, 2025, 10, 5));
    auto result = svc->compute(empId, 2025);
    LOG_DEBUG(result.to_string());

    EXPECT_EQ(result.unusedLeaveDays, 5.0);
}

TEST_F(YearEndBenefitsServiceTest, ComputesDailyRateCorrectly)
{

    leaveRepo->insert(makeLeaveBalance("00-0001", 2025, 10.0, 0.0));

    auto result = svc->compute("00-0001", 2025);

    double expectedDailyRate = 45000.0 * 12.0 / 314.0;
    EXPECT_NEAR(result.dailyRate, expectedDailyRate, 0.01);
}

TEST_F(YearEndBenefitsServiceTest, ComputesMonetizedLeaveValueCorrectly)
{

    leaveRepo->insert(makeLeaveBalance("00-0001", 2025, 10.0, 3.0));

    auto result = svc->compute("00-0001", 2025);

    double expectedDailyRate = 45000.0 * 12.0 / 314.0;
    double expectedMonetized = expectedDailyRate * 7.0;
    EXPECT_NEAR(result.monetizedLeaveValue, expectedMonetized, 0.01);
}
