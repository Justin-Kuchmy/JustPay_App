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

TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_ReturnsValidId)
{
    DepartmentBudget dBudget(0, Department::Operations, 1, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);

    DepartmentBudget dBudget2(1, Department::Sales, 1, 30000, "notes go here");
    int id2 = repo->insertDepartmentBudget(dBudget2);
    EXPECT_GT(id2, 0);
}

TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_AllowsMultipleDepartmentsSamePeriod)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);
}
TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_AllowsSameDepartmentDifferentPeriods)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);
}
TEST_F(DepartmentBudgetRepoTest, InsertDepartmentBudget_FailsForDuplicateDepartmentAndPeriod)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);
}

TEST_F(DepartmentBudgetRepoTest, GetById_ReturnsCorrectBudget)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);
    auto newOpt = repo->getById(id);
    EXPECT_TRUE(newOpt.has_value());
    DepartmentBudget newdB = *newOpt;
    EXPECT_EQ(newdB.department, Department::Operations);
    EXPECT_EQ(newdB.period_Id, dBudget.period_Id);
    EXPECT_EQ(newdB.id, id);
}
TEST_F(DepartmentBudgetRepoTest, GetById_ReturnsNullopt_WhenNotFound)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    auto newOpt = repo->getById(id + 1);
    EXPECT_EQ(newOpt, std::nullopt);
}
TEST_F(DepartmentBudgetRepoTest, GetByPeriod_ReturnsBudgetsForSpecificPeriod)
{
    DepartmentBudget dBudget1(0, Department::Operations, 1, 50000, "notes go here");
    DepartmentBudget dBudget2(0, Department::Admin, 1, 50000, "notes go here");
    DepartmentBudget dBudget3(0, Department::Operations, 2, 50000, "notes go here");

    int id1 = repo->insertDepartmentBudget(dBudget1);
    int id2 = repo->insertDepartmentBudget(dBudget2);
    int id3 = repo->insertDepartmentBudget(dBudget3);
    EXPECT_GT(id1, 0);
    EXPECT_GT(id2, 0);
    EXPECT_GT(id3, 0);

    std::vector<DepartmentBudget> opResult = repo->getByPeriod(1);
    std::vector<DepartmentBudget> adminResult = repo->getByPeriod(2);
    EXPECT_EQ(opResult.size(), 5u);
    EXPECT_EQ(adminResult.size(), 1u);
}
TEST_F(DepartmentBudgetRepoTest, GetByPeriod_ReturnsEmpty_WhenNoBudgetsExist)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);

    std::vector<DepartmentBudget> hrResult = repo->getByPeriod(99);
    EXPECT_EQ(hrResult.size(), 0);
}
TEST_F(DepartmentBudgetRepoTest, GetAll_ReturnsAllBudgets)
{
    DepartmentBudget dBudget1(0, Department::Operations, 1, 50000, "notes go here");
    DepartmentBudget dBudget2(0, Department::Admin, 1, 50000, "notes go here");
    DepartmentBudget dBudget3(0, Department::Operations, 2, 50000, "notes go here");

    int id1 = repo->insertDepartmentBudget(dBudget1);
    int id2 = repo->insertDepartmentBudget(dBudget2);
    int id3 = repo->insertDepartmentBudget(dBudget3);
    EXPECT_GT(id1, 0);
    EXPECT_GT(id2, 0);
    EXPECT_GT(id3, 0);

    std::vector<DepartmentBudget> allObjects = repo->getAll();
    EXPECT_EQ(allObjects.size(), 6u);
}
TEST_F(DepartmentBudgetRepoTest, UpdateDepartmentBudget_ModifiesExistingRecord)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);

    auto newOpt = repo->getById(id);
    ASSERT_TRUE(newOpt.has_value());

    DepartmentBudget newObj = *newOpt;
    newObj.allocatedAmount = 60000;
    EXPECT_TRUE(repo->updateDepartmentBudget(newObj));

    auto updatedOpt = repo->getById(id);
    ASSERT_TRUE(updatedOpt.has_value());
    EXPECT_EQ(updatedOpt->allocatedAmount, 60000);
}

TEST_F(DepartmentBudgetRepoTest, DeleteDepartmentBudget_RemovesRecord)
{
    DepartmentBudget dBudget(0, Department::Operations, 0, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_TRUE(repo->deleteDepartmentBudget(id));
    auto newOpt = repo->getById(id);
    EXPECT_FALSE(newOpt.has_value());
}
TEST_F(DepartmentBudgetRepoTest, DeleteDepartmentBudget_ReturnsFalse_WhenIdDoesNotExist)
{
    DepartmentBudget dBudget(0, Department::Operations, 1, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_FALSE(repo->deleteDepartmentBudget(id + 99));
}

TEST_F(DepartmentBudgetRepoTest, UpdateDepartmentBudget_ReturnsFalse_WhenIdDoesNotExist)
{
    DepartmentBudget dBudget(0, Department::Operations, 1, 50000, "notes go here");
    int id = repo->insertDepartmentBudget(dBudget);
    EXPECT_GT(id, 0);

    auto newOpt = repo->getById(id);
    ASSERT_TRUE(newOpt.has_value());

    DepartmentBudget newObj = *newOpt;
    newObj.allocatedAmount = 60000;
    EXPECT_TRUE(repo->deleteDepartmentBudget(newObj.id));
    EXPECT_FALSE(repo->updateDepartmentBudget(newObj));
}
