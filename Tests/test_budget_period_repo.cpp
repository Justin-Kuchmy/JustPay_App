#include <gtest/gtest.h>
#include "Repositories/BudgetPeriodRepository.h"
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

class BudgetPeriodRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<BudgetPeriodRepository> repo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<BudgetPeriodRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(BudgetPeriodRepoTest, InsertBudgetPeriod_ReturnsValidId)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    EXPECT_GT(id, 0);
}

TEST_F(BudgetPeriodRepoTest, GetById_ReturnsCorrectPeriod)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    auto newOpt = repo->getById(id);
    EXPECT_TRUE(newOpt.has_value());
    BudgetPeriod newBp = *newOpt;
    EXPECT_EQ("2026-05-01", newBp.startDate.to_string());
    EXPECT_EQ("2026-05-15", newBp.endDate.to_string());
    EXPECT_EQ(1, newBp.half);
}
TEST_F(BudgetPeriodRepoTest, GetById_ReturnsNullopt_WhenNotFound)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    auto newOpt = repo->getById(id + 1);
    EXPECT_EQ(newOpt, std::nullopt);
}

TEST_F(BudgetPeriodRepoTest, GetAll_ReturnsAllInsertedPeriods)
{
    auto vBudgets = repo->getAll();
    EXPECT_EQ(vBudgets.size(), 2u);
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    EXPECT_GT(id, 0);
    vBudgets = repo->getAll();
    EXPECT_EQ(vBudgets.size(), 3u);
}

TEST_F(BudgetPeriodRepoTest, InsertBudgetPeriod_AllowsDifferentYears)
{
    BudgetPeriod bp1{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    BudgetPeriod bp2{0, "May 2026 - First Half", 2027, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id1 = repo->insertBudgetPeriod(bp1);
    int id2 = repo->insertBudgetPeriod(bp2);
    EXPECT_GT(id1, 0);
    EXPECT_GT(id2, 0);
}
TEST_F(BudgetPeriodRepoTest, InsertBudgetPeriod_AllowsSameYearDifferentHalf)
{
    BudgetPeriod bp1{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    BudgetPeriod bp2{0, "May 2026 - First Half", 2026, 2, Date{2026, 05, 16}, Date{2026, 05, 31}};
    int id1 = repo->insertBudgetPeriod(bp1);
    int id2 = repo->insertBudgetPeriod(bp2);
    EXPECT_GT(id1, 0);
    EXPECT_GT(id2, 0);
}

TEST_F(BudgetPeriodRepoTest, UpdateBudgetPeriod_ModifiesExistingRecord)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 5, 1}, Date{2026, 5, 15}};
    int id = repo->insertBudgetPeriod(bp);
    EXPECT_GT(id, 0);

    auto newOpt = repo->getById(id);
    ASSERT_TRUE(newOpt.has_value());

    BudgetPeriod newObj = *newOpt;
    newObj.label = "some new label";

    EXPECT_TRUE(repo->updateBudgetPeriod(newObj));

    auto updatedOpt = repo->getById(id);
    ASSERT_TRUE(updatedOpt.has_value());

    EXPECT_EQ(updatedOpt->label, "some new label");
}

TEST_F(BudgetPeriodRepoTest, DeleteBudgetPeriod_RemovesRecord)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    EXPECT_TRUE(repo->deleteBudgetPeriod(id));
    auto newOpt = repo->getById(id);
    EXPECT_FALSE(newOpt.has_value());
}

TEST_F(BudgetPeriodRepoTest, DeleteBudgetPeriod_ReturnsFalse_WhenIdDoesNotExist)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    EXPECT_FALSE(repo->deleteBudgetPeriod(id + 99));
}

TEST_F(BudgetPeriodRepoTest, UpdateBudgetPeriod_ReturnsFalse_WhenIdDoesNotExist)
{
    BudgetPeriod bp{0, "May 2026 - First Half", 2026, 1, Date{2026, 05, 01}, Date{2026, 05, 15}};
    int id = repo->insertBudgetPeriod(bp);
    EXPECT_GT(id, 0);

    auto newOpt = repo->getById(id);
    EXPECT_TRUE(newOpt.has_value());
    BudgetPeriod newObj = *newOpt;
    newObj.label = "some new label";

    EXPECT_TRUE(repo->deleteBudgetPeriod(newObj.id));
    EXPECT_FALSE(repo->updateBudgetPeriod(newObj));
}