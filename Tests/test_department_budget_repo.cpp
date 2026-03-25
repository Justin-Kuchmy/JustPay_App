#include <gtest/gtest.h>
#include "Repositories/DepartmentBudgetRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>

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

class DepartmentBudgetRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<DepartmentBudgetRepository> repo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<DepartmentBudgetRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_SucceedsWithValidData)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_ReturnsValidId)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, GetById_ReturnsCorrectBudget)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, GetById_ReturnsNullopt_WhenNotFound)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, GetByPeriod_ReturnsBudgetsForSpecificPeriod)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, GetByPeriod_ReturnsEmpty_WhenNoBudgetsExist)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, GetAll_ReturnsAllBudgets)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, UpdateDepartmentBudget_ModifiesExistingRecord)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, UpdateDepartmentBudget_ReturnsFalse_WhenIdDoesNotExist)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, DeleteDepartmentBudget_RemovesRecord)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, DeleteDepartmentBudget_ReturnsFalse_WhenIdDoesNotExist)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_AllowsMultipleDepartmentsSamePeriod)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_AllowsSameDepartmentDifferentPeriods)
{
    EXPECT_EQ(1, 0);
}
TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_FailsForDuplicateDepartmentAndPeriod)
{
    EXPECT_EQ(1, 0);
}