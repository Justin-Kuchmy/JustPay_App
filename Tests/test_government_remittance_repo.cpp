#include <gtest/gtest.h>
#include "Repositories/GovernmentRemittanceRepository.h"
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

class GovernmentRemittanceRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<GovernmentRemittanceRepository> repo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<GovernmentRemittanceRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

// Helper to build a fully populated GovernmentRemittance
static GovernmentRemittance makeRemittance(
    const std::string &employeeId = "01-0099",
    const std::string &period = "March 2026",
    int half = 1,
    int payrollId = 1)
{
    GovernmentRemittance r;
    r.payrollCalculationResultsId = payrollId;
    r.employeeId = employeeId;
    r.fullName = "Juan dela Cruz";
    r.employeeDepartment = "Engineering";
    r.payPeriodText = period;
    r.payPeriodHalf = half;

    r.sssPremium_EE = 581.30;
    r.sssPremium_ER = 1208.70;
    r.sssPremiumTotal = r.sssPremium_EE + r.sssPremium_ER;

    r.philHealthPremium_EE = 450.00;
    r.philHealthPremium_ER = 450.00;
    r.philHealthPremiumTotal = r.philHealthPremium_EE + r.philHealthPremium_ER;

    r.hdmfPremium_EE = 100.00;
    r.hdmfPremium_ER = 100.00;
    r.hdmfPremiumTotal = r.hdmfPremium_EE + r.hdmfPremium_ER;

    r.withHoldingTax = 1200.00;

    r.sssSubmissionStatus = RemittanceStatus::PENDING;
    r.phicSubmissionStatus = RemittanceStatus::PENDING;
    r.hdmfSubmissionStatus = RemittanceStatus::PENDING;
    r.withHoldingTaxSubmissionStatus = RemittanceStatus::PENDING;

    r.lastSubmittedDate = Date(1970, 1, 1);
    r.submittedByUserId = 0;
    r.dateCreated = Date(2026, 3, 15);
    r.dateModified = Date(2026, 3, 15);
    return r;
}

// // ─── Insert ──────────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, InsertRemittance_Succeeds)
{
    auto id = repo->insertRemittance(makeRemittance());
    EXPECT_GT(id, 0);
}

TEST_F(GovernmentRemittanceRepoTest, InsertRemittanceReports_Batch_ReturnsAllIds)
{
    std::vector<GovernmentRemittance> batch = {
        makeRemittance("01-0099", "March 2026", 1, 1),
        makeRemittance("01-0100", "March 2026", 1, 2),
        makeRemittance("02-0101", "March 2026", 1, 3)};

    auto ids = repo->insertRemittanceReports(batch);
    EXPECT_EQ(ids.size(), 3u);
    for (auto id : ids)
        EXPECT_GT(id, 0);
}

// // ─── GetById ─────────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetById_ReturnsNulloptIfNotFound)
{
    auto result = repo->getRemittanceById(99999);
    EXPECT_FALSE(result.has_value());
}

TEST_F(GovernmentRemittanceRepoTest, GetById_ReturnsCorrectRecord)
{
    auto r = makeRemittance("01-0099", "March 2026", 1);
    auto id = repo->insertRemittance(r);
    ASSERT_GT(id, 0);
    LOG_DEBUG("id " << id);

    auto result = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->employeeId, r.employeeId);
    EXPECT_EQ(result->payPeriodText, r.payPeriodText);
    EXPECT_EQ(result->payPeriodHalf, r.payPeriodHalf);
}

// ─── GetByPayrollId ───────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetByPayrollId_ReturnsCorrectRecord)
{
    auto r = makeRemittance("01-0099", "March 2026", 1, 42);
    auto id = repo->insertRemittance(r);
    ASSERT_GT(id, 0);

    auto result = repo->getRemittanceByPayrollId(42);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->payrollCalculationResultsId, 42);
}

TEST_F(GovernmentRemittanceRepoTest, GetByPayrollId_ReturnsNulloptIfNotFound)
{
    auto result = repo->getRemittanceByPayrollId(99999);
    EXPECT_FALSE(result.has_value());
}

// ─── GetByPeriod ─────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetByPeriod_ReturnsAllForPeriod)
{
    repo->insertRemittance(makeRemittance("01-0099", "March 2026", 1));
    repo->insertRemittance(makeRemittance("01-0100", "March 2026", 1));
    repo->insertRemittance(makeRemittance("01-0099", "April 2026", 1)); // different period

    auto results = repo->getRemittancesByPeriod("March 2026");
    EXPECT_EQ(results.size(), 2u);
    for (const auto &r : results)
        EXPECT_EQ(r.payPeriodText, "March 2026");
}

TEST_F(GovernmentRemittanceRepoTest, GetByPeriod_FilteredByHalf)
{
    repo->insertRemittance(makeRemittance("01-0099", "March 2026", 1));
    repo->insertRemittance(makeRemittance("01-0100", "March 2026", 2));
    repo->insertRemittance(makeRemittance("02-0101", "March 2026", 2));

    auto firstHalf = repo->getRemittancesByPeriod("March 2026", 1);
    auto secondHalf = repo->getRemittancesByPeriod("March 2026", 2);

    EXPECT_EQ(firstHalf.size(), 1u);
    EXPECT_EQ(secondHalf.size(), 2u);
}

TEST_F(GovernmentRemittanceRepoTest, GetByPeriod_ReturnsEmptyForUnknownPeriod)
{
    auto results = repo->getRemittancesByPeriod("January 2000");
    EXPECT_TRUE(results.empty());
}

// ─── GetByEmployee ────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetByEmployee_ReturnsAllForEmployee)
{
    repo->insertRemittance(makeRemittance("01-0099", "March 2026", 1, 1));
    repo->insertRemittance(makeRemittance("01-0099", "April 2026", 1, 2));
    repo->insertRemittance(makeRemittance("01-0100", "March 2026", 1, 3));

    auto results = repo->getRemittancesByEmployee("01-0099");
    EXPECT_EQ(results.size(), 2u);
    for (const auto &r : results)
        EXPECT_EQ(r.employeeId, "01-0099");
}

// ─── GetByStatus ─────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetPending_ReturnsOnlyPendingRecords)
{
    repo->insertRemittance(makeRemittance("01-0099", "March 2026", 1, 1));
    repo->insertRemittance(makeRemittance("01-0100", "March 2026", 1, 2));

    auto pending = repo->getRemittancesByStatus(RemittanceStatus::PENDING);
    EXPECT_EQ(pending.size(), 2u);
}

// ─── GetAll ───────────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetAll_ReturnsAllRecords)
{
    repo->insertRemittance(makeRemittance("01-0099", "March 2026", 1, 1));
    repo->insertRemittance(makeRemittance("01-0100", "March 2026", 1, 2));
    repo->insertRemittance(makeRemittance("02-0101", "April 2026", 2, 3));

    auto all = repo->getAllRemittances();
    EXPECT_EQ(all.size(), 3u);
}

// ─── Update ───────────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, UpdateRemittance_ModifiesExistingRecord)
{
    auto id = repo->insertRemittance(makeRemittance());
    ASSERT_GT(id, 0);

    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());

    fetched->withHoldingTax = 9999.99;
    bool ok = repo->updateRemittance(fetched.value());
    EXPECT_TRUE(ok);

    auto updated = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(updated.has_value());
    EXPECT_NEAR(updated->withHoldingTax, 9999.99, 0.1);
}

// ─── Status transitions ───────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, MarkAsSubmitted_UpdatesStatusAndAuditFields)
{
    auto id = repo->insertRemittance(makeRemittance());
    ASSERT_GT(id, 0);

    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());

    fetched->sssSubmissionStatus = RemittanceStatus::SUBMITTED;
    fetched->dateModified = Date::getTodayDate();
    fetched->lastSubmittedDate = Date::getTodayDate();
    bool ok = repo->markAsSubmitted(fetched->id, RemittanceType::SSS);
    EXPECT_TRUE(ok);

    auto result = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->sssSubmissionStatus, RemittanceStatus::SUBMITTED);
    EXPECT_EQ(result->lastSubmittedDate.year, 2026);
    EXPECT_EQ(result->lastSubmittedDate.month, 3);
    EXPECT_EQ(result->lastSubmittedDate.day, 18);

    EXPECT_EQ(result->dateModified.year, 2026);
    EXPECT_EQ(result->dateModified.month, 3);
    EXPECT_EQ(result->dateModified.day, 18);
}

TEST_F(GovernmentRemittanceRepoTest, MarkAsConfirmed_UpdatesStatus)
{
    auto id = repo->insertRemittance(makeRemittance());
    ASSERT_GT(id, 0);

    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());

    bool ok = repo->markAsConfirmed(fetched->id, RemittanceType::HDMF);
    EXPECT_TRUE(ok);

    auto result = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->hdmfSubmissionStatus, RemittanceStatus::CONFIRMED);

    EXPECT_EQ(result->dateModified.year, 2026);
    EXPECT_EQ(result->dateModified.month, 3);
    EXPECT_EQ(result->dateModified.day, 18);
}

TEST_F(GovernmentRemittanceRepoTest, MarkAsRejected_UpdatesStatus)
{
    auto id = repo->insertRemittance(makeRemittance());
    ASSERT_GT(id, 0);

    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());
    LOG_DEBUG(fetched->to_string());

    bool ok = repo->markAsRejected(fetched->id, RemittanceType::PHIC);
    EXPECT_TRUE(ok);

    auto result = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(result.has_value());
    LOG_DEBUG(result->to_string());
    EXPECT_EQ(result->phicSubmissionStatus, RemittanceStatus::REJECTED);

    EXPECT_EQ(result->dateModified.year, 2026);
    EXPECT_EQ(result->dateModified.month, 3);
    EXPECT_EQ(result->dateModified.day, 18);
}

// ─── Delete ───────────────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, DeleteRemittance_RemovesRecord)
{
    auto id = repo->insertRemittance(makeRemittance());
    ASSERT_GT(id, 0);

    bool ok = repo->deleteRemittance(static_cast<int>(id));
    EXPECT_TRUE(ok);

    auto result = repo->getRemittanceById(static_cast<int>(id));
    EXPECT_FALSE(result.has_value());
}

// ─── Model helper methods ─────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, TotalEmployeeContribution_SumsCorrectly)
{
    auto r = makeRemittance();
    auto id = repo->insertRemittance(r);
    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());

    double expected = fetched->sssPremium_EE + fetched->philHealthPremium_EE + fetched->hdmfPremium_EE;
    EXPECT_NEAR(fetched->totalEmployeeContribution(), expected, 0.01);
}

TEST_F(GovernmentRemittanceRepoTest, TotalEmployerContribution_SumsCorrectly)
{
    auto r = makeRemittance();
    auto id = repo->insertRemittance(r);
    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());

    double expected = fetched->sssPremium_ER + fetched->philHealthPremium_ER + fetched->hdmfPremium_ER;
    EXPECT_NEAR(fetched->totalEmployerContribution(), expected, 0.01);
}

TEST_F(GovernmentRemittanceRepoTest, TotalGovernmentRemittance_SumsCorrectly)
{
    auto r = makeRemittance();
    auto id = repo->insertRemittance(r);
    auto fetched = repo->getRemittanceById(static_cast<int>(id));
    ASSERT_TRUE(fetched.has_value());

    double expected = fetched->totalEmployeeContribution() + fetched->totalEmployerContribution() + fetched->withHoldingTax;
    EXPECT_NEAR(fetched->totalGovernmentRemittance(), expected, 0.01);
}

// ─── Monthly summary ─────────────────────────────────────────────────────────

TEST_F(GovernmentRemittanceRepoTest, GetMonthlySummary_AggregatesCorrectly)
{
    repo->insertRemittance(makeRemittance("01-0099", "March 2026", 1, 1));
    repo->insertRemittance(makeRemittance("01-0100", "March 2026", 1, 2));

    auto summary = repo->getMonthlySummary("March 2026");
    EXPECT_EQ(summary.monthYear, "March 2026");
    EXPECT_NEAR(summary.totalSSSEE, 581.30 * 2, 0.01);
    EXPECT_NEAR(summary.totalPHICEE, 450.00 * 2, 0.01);
    EXPECT_NEAR(summary.totalHDMFEE, 100.00 * 2, 0.01);
}

TEST_F(GovernmentRemittanceRepoTest, GetMonthlySummary_ReturnsEmptyForUnknownMonth)
{
    auto summary = repo->getMonthlySummary("January 2000");
    EXPECT_EQ(summary.monthYear, "January 2000");
    EXPECT_DOUBLE_EQ(summary.totalSSSEE, 0.0);
    EXPECT_DOUBLE_EQ(summary.totalPHICEE, 0.0);
    EXPECT_DOUBLE_EQ(summary.totalHDMFEE, 0.0);
}