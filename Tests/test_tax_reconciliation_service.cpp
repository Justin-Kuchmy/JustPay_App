#include <gtest/gtest.h>
#include "Services/YearEndBenefitsService.h"
#include "Services/LeaveBalanceService.h"
#include "Services/GovernmentRemittanceService.h"
#include "Services/PayrollService.h"
#include "Services/EmployeeService.h"
#include "Services/TaxReconciliationService.h"
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

class TaxReconciliationDbTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;

    std::unique_ptr<PayrollRepository> payrollRepo;
    std::unique_ptr<LeaveRepository> leaveRepo;
    std::unique_ptr<EmployeeRepository> empRepo;
    std::unique_ptr<GovernmentRemittanceRepository> remitRepo;

    std::unique_ptr<EmployeeService> employeeService;
    std::unique_ptr<PayrollService> payrollService;
    std::unique_ptr<YearEndBenefitsService> yearEndService;
    std::unique_ptr<GovernmentRemittanceService> govtRemittService;
    std::unique_ptr<LeaveBalanceService> leaveBalanceService;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        employeeService.reset();
        payrollService.reset();
        leaveBalanceService.reset();
        yearEndService.reset();
        govtRemittService.reset();

        empRepo = std::make_unique<EmployeeRepository>(db);
        payrollRepo = std::make_unique<PayrollRepository>(db);
        remitRepo = std::make_unique<GovernmentRemittanceRepository>(db);
        leaveRepo = std::make_unique<LeaveRepository>(db);

        employeeService = std::make_unique<EmployeeService>(*empRepo);
        payrollService = std::make_unique<PayrollService>(*payrollRepo);
        leaveBalanceService = std::make_unique<LeaveBalanceService>(*leaveRepo, *empRepo);
        yearEndService = std::make_unique<YearEndBenefitsService>(*payrollRepo, *leaveBalanceService, *empRepo);
        govtRemittService = std::make_unique<GovernmentRemittanceService>(*remitRepo, *payrollRepo);
    }

    void TearDown() override
    {
        employeeService.reset();
        payrollService.reset();
        leaveBalanceService.reset();
        yearEndService.reset();
        govtRemittService.reset();
        payrollRepo.reset();
        leaveRepo.reset();
        empRepo.reset();
        remitRepo.reset();
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

class TaxReconciliationServiceTest : public TaxReconciliationDbTest
{
protected:
    std::unique_ptr<TaxReconciliationService> svc;

    void SetUp() override
    {
        TaxReconciliationDbTest::SetUp();
        svc = std::make_unique<TaxReconciliationService>(*employeeService, *payrollService, *yearEndService, *govtRemittService);
    }

    void TearDown() override
    {
        svc.reset();
        TaxReconciliationDbTest::TearDown();
    }
};
