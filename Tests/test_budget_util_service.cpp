#include <gtest/gtest.h>
#include "Services/PayrollService.h"
#include "Services/BudgetUtilService.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include <algorithm>

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
    std::unique_ptr<PayrollRepository> pRepo;
    std::unique_ptr<GovernmentRemittanceRepository> gRepo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        pRepo = std::make_unique<PayrollRepository>(db);
        gRepo = std::make_unique<GovernmentRemittanceRepository>(db);
    }

    void TearDown() override
    {
        pRepo.reset();
        gRepo.reset();
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class BudgetUtilServiceTest : public PayrollDbTest
{
protected:
    std::unique_ptr<PayrollService> payrollService;
    std::unique_ptr<BudgetUtilService> budgetUtilService;
    std::unique_ptr<GovernmentRemittanceService> govRemitService;

    void SetUp() override
    {
        PayrollDbTest::SetUp();
        payrollService = std::make_unique<PayrollService>(*pRepo);
        govRemitService = std::make_unique<GovernmentRemittanceService>(*gRepo, *pRepo);
        budgetUtilService = std::make_unique<BudgetUtilService>(*payrollService, *govRemitService);
    }
    void TearDown() override
    {
        budgetUtilService.reset();
        govRemitService.reset();
        payrollService.reset();
        PayrollDbTest::TearDown();
    }
};

TEST_F(BudgetUtilServiceTest, GetAll_ReturnsAllPayrollRecords)
{
    auto vP1 = pRepo->getAll();
    EXPECT_TRUE(vP1.size() == 20);
}

TEST_F(BudgetUtilServiceTest, ReturnsTotalDepartmentCountInPayollPeriod)
{
    auto results = budgetUtilService->getMonthlyBudget("2025 01");
    EXPECT_EQ(results.size(), 6);
}

TEST_F(BudgetUtilServiceTest, ReturnsAccurateTotalAllowancesForGivenDepartment)
{
    auto results = budgetUtilService->getMonthlyBudget("2025 01");
    int departmentID{Department::Sales};
    std::vector<MonthlyBudgetUtilizationReport> budgetMatches;
    std::ranges::copy_if(results, std::back_inserter(budgetMatches), [&departmentID](const MonthlyBudgetUtilizationReport &r)
                         { return r.departmentName == department_to_string(departmentID); });
    EXPECT_GT(budgetMatches.size(), 0u);
    EXPECT_NEAR(budgetMatches.front().totalAllowances, 4200.00, 0.01);
}