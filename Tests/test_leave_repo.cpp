#include <gtest/gtest.h>
#include "Repositories/LeaveRepository.h"
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

class LeaveRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<LeaveRepository> repo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<LeaveRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(LeaveRepoTest, GetByEmpYear_ReturnsEmployeeLeaveBalanceIfExists)
{
    EXPECT_TRUE(repo->insert(EmployeeLeaveBalance("00-0001", 2025)));
    std::optional<EmployeeLeaveBalance> bal = repo->getByEmployeeAndYear("00-0001", 2025);
    EXPECT_TRUE(bal.has_value());
}

TEST_F(LeaveRepoTest, GetByEmpYear_ReturnsEmployeeLeaveBalanceWithCorrectValues)
{
    EXPECT_TRUE(repo->insert(EmployeeLeaveBalance("00-0001", 2025, 10.0, 5.0)));
    std::optional<EmployeeLeaveBalance> bal = repo->getByEmployeeAndYear("00-0001", 2025);
    EmployeeLeaveBalance result = bal.has_value() ? *bal : EmployeeLeaveBalance{};
    EXPECT_TRUE(bal.has_value());
    EXPECT_TRUE(result.employeeId == "00-0001");
    EXPECT_TRUE(result.totalLeaveEarned == 10.0);
    EXPECT_TRUE(result.leaveUsed == 5.0);
    EXPECT_TRUE(result.unusedLeaveDays == 5.0);
    EXPECT_TRUE(result.year == 2025);
}

TEST_F(LeaveRepoTest, GetByEmpYear_ReturnsNullIfDoesntExist)
{
    std::optional<EmployeeLeaveBalance> bal = repo->getByEmployeeAndYear("00-9999", 9999);
    EmployeeLeaveBalance result{};
    EXPECT_FALSE(bal.has_value());
}