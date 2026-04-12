#include <gtest/gtest.h>
#include "Services/GovernmentRemittanceService.h"
#include "Repositories/GovernmentRemittanceRepository.h"
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

class GovRemitDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<GovernmentRemittanceRepository> remitRepo;
    std::unique_ptr<PayrollRepository> payrollRepo;
    std::unique_ptr<GovernmentRemittanceService> remitService;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        remitService.reset();

        remitRepo = std::make_unique<GovernmentRemittanceRepository>(db);
        payrollRepo = std::make_unique<PayrollRepository>(db);
        remitService = std::make_unique<GovernmentRemittanceService>(*remitRepo, *payrollRepo);
    }

    void TearDown() override
    {
        remitService.reset();
        remitRepo.reset();
        payrollRepo.reset();
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class GovRemitServiceTest : public GovRemitDbTest
{
protected:
    std::unique_ptr<GovernmentRemittanceService> service;

    void SetUp() override
    {
        GovRemitDbTest::SetUp();
        service = std::make_unique<GovernmentRemittanceService>(*remitRepo, *payrollRepo);
    }

    void TearDown() override
    {
        service.reset();
        GovRemitDbTest::TearDown();
    }
};

TEST_F(GovRemitServiceTest, firstTest)
{

    EXPECT_TRUE(1 == 1);
}