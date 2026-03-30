#include <gtest/gtest.h>
#include "Services/LeaveBalanceService.h"
#include "Repositories/LeaveRepository.h"
#include "Repositories/EmployeeRepository.h"
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

class LeaveBalanceDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<EmployeeRepository> empRepo;
    std::unique_ptr<LeaveRepository> leaveRepo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        empRepo = std::make_unique<EmployeeRepository>(db);
        leaveRepo = std::make_unique<LeaveRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class LeaveBalanceServiceTest : public LeaveBalanceDbTest
{
protected:
    std::unique_ptr<LeaveBalanceService> svc;

    void SetUp() override
    {
        LeaveBalanceDbTest::SetUp();
        svc = std::make_unique<LeaveBalanceService>(*leaveRepo, *empRepo);
    }

    void TearDown() override
    {
        svc.reset();
        LeaveBalanceDbTest::TearDown();
    }
};

static Employee makeEmployee(const std::string &fullName, const std::string &dateHired, double salary = 20000.0)
{
    Employee emp{};
    emp.fullName = fullName;
    emp.department = Department(0);
    emp.position = "Staff";
    emp.jobLevel = JobLevel(1);
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

TEST_F(LeaveBalanceServiceTest, FullEntitlement_HiredBeforePreviousCutOff)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Bob Reyes", "2022-01-15"));
    double entitlement = svc->computeLeaveEntitlement(empId, 2025);
    EXPECT_DOUBLE_EQ(entitlement, 10.0);
}

TEST_F(LeaveBalanceServiceTest, FullEntitlement_RegularizationExactlyOnPreviousCutOff)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Carlo Mendoza", "2024-05-31"));
    double entitlement = svc->computeLeaveEntitlement(empId, 2025);
    EXPECT_DOUBLE_EQ(entitlement, 10.0);
}

TEST_F(LeaveBalanceServiceTest, ProRated_RegularizedJuly)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Dana Cruz", "2025-01-01"));
    double entitlement = svc->computeLeaveEntitlement(empId, 2025);
    double expected = 10.0 * (153.0 / 365.0);
    EXPECT_NEAR(entitlement, expected, 0.001);
}

TEST_F(LeaveBalanceServiceTest, ProRated_RegularizedNovember)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Ella Bautista", "2025-05-01"));
    double entitlement = svc->computeLeaveEntitlement(empId, 2025);
    double expected = 10.0 * (30.0 / 365.0);
    EXPECT_NEAR(entitlement, expected, 0.001);
}

TEST_F(LeaveBalanceServiceTest, ProRated_RegularizedOctober)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Frank Lim", "2025-04-15"));
    double entitlement = svc->computeLeaveEntitlement(empId, 2025);
    double expected = 10.0 * (47.0 / 365.0);
    EXPECT_NEAR(entitlement, expected, 0.001);
}

TEST_F(LeaveBalanceServiceTest, FullEntitlement_EmployeeNotFound_ReturnsZero)
{
    double entitlement = svc->computeLeaveEntitlement("99-9999", 2025);
    EXPECT_DOUBLE_EQ(entitlement, 0.0);
}

TEST_F(LeaveBalanceServiceTest, UnusedLeave_NoLeavesUsed)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Grace Tan", "2022-01-15"));
    leaveRepo->insert(makeLeaveBalance(empId, 2025, 10.0, 0.0));

    double unused = svc->computeUnusedLeave(empId, 2025);
    EXPECT_DOUBLE_EQ(unused, 10.0);
}

TEST_F(LeaveBalanceServiceTest, UnusedLeave_SomeLeavesUsed)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Henry Go", "2022-01-15"));
    leaveRepo->insert(makeLeaveBalance(empId, 2025, 10.0, 3.5));

    double unused = svc->computeUnusedLeave(empId, 2025);
    EXPECT_DOUBLE_EQ(unused, 6.5);
}

TEST_F(LeaveBalanceServiceTest, UnusedLeave_UsedMoreThanEntitled_ClampsToZero)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Iris Dela Rosa", "2025-04-15"));
    leaveRepo->insert(makeLeaveBalance(empId, 2025, 1.26, 3.0));

    double unused = svc->computeUnusedLeave(empId, 2025);
    EXPECT_DOUBLE_EQ(unused, 0.0);
}

TEST_F(LeaveBalanceServiceTest, UnusedLeave_NoBalanceRecord_TreatsUsedAsZero)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Joel Santos", "2022-01-15"));

    double unused = svc->computeUnusedLeave(empId, 2025);
    EXPECT_DOUBLE_EQ(unused, 10.0);
}

TEST_F(LeaveBalanceServiceTest, UnusedLeave_EmployeeNotFound_ReturnsZero)
{
    double unused = svc->computeUnusedLeave("99-9999", 2025);
    EXPECT_DOUBLE_EQ(unused, 0.0);
}

TEST_F(LeaveBalanceServiceTest, ComputeLeaveBalance_PopulatesStructCorrectly)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Karen Villanueva", "2022-01-15"));
    leaveRepo->insert(makeLeaveBalance(empId, 2025, 10.0, 2.0));

    EmployeeLeaveBalance balance = svc->computeLeaveBalance(empId, 2025);

    EXPECT_EQ(balance.employeeId, empId);
    EXPECT_EQ(balance.year, 2025);
    EXPECT_DOUBLE_EQ(balance.totalLeaveEarned, 10.0);
    EXPECT_DOUBLE_EQ(balance.leaveUsed, 2.0);
    EXPECT_DOUBLE_EQ(balance.unusedLeaveDays, 8.0);
}

TEST_F(LeaveBalanceServiceTest, ComputeLeaveBalance_ProRated_PopulatesStructCorrectly)
{
    std::string empId = empRepo->insertEmployee(makeEmployee("Leo Reyes", "2025-04-15"));

    EmployeeLeaveBalance balance = svc->computeLeaveBalance(empId, 2025);

    double expected = 10.0 * (47.0 / 365.0);
    EXPECT_EQ(balance.employeeId, empId);
    EXPECT_EQ(balance.year, 2025);
    EXPECT_NEAR(balance.totalLeaveEarned, expected, 0.001);
    EXPECT_DOUBLE_EQ(balance.leaveUsed, 0.0);
    EXPECT_NEAR(balance.unusedLeaveDays, expected, 0.001);
}