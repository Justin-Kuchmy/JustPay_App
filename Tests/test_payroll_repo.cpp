#include <gtest/gtest.h>
#include "Repositories/PayrollRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"

// ASSERT_EQ(a, b)   // a == b
// ASSERT_NE(a, b)   // a != b
// ASSERT_GT(a, b)   // a > b
// ASSERT_GE(a, b)   // a >= b
// ASSERT_LT(a, b)   // a < b
// ASSERT_LE(a, b)   // a <= b

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

static PayrollCalculationResults makePayroll(const std::string &employeeId = "00-0001", const std::string &period = "2026 01", int half = 1)
{
    PayrollCalculationResults p;
    p.employeeId = employeeId;
    p.fullName = "Alice Santos";
    p.employeeDepartment = 0;
    p.payPeriodDate = period;
    p.payPeriodHalf = half;
    p.monthlyBasicSalary = 30000.0;
    p.monthlyAllowances = 2000.0;
    p.overTimePay = 500.0;
    p.adjustments = 0.0;
    p.grossIncome = 32500.0;
    p.sssPremium_EE = 581.30;
    p.philHealthPremium_EE = 450.00;
    p.hdmfPremium_EE = 100.00;
    p.loanDeductionsPerPayroll = 0.0;
    p.withHoldingTax = 1200.0;
    p.totalDeductions = 2331.30;
    p.netPay = 30168.70;
    p.dateProcessed = Date::getTodayDate();
    p.sssPremium_ER = 581.30;
    p.philHealthPremium_ER = 450.00;
    p.hdmfPremium_ER = 100.00;
    return p;
}

class PayrollRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<PayrollRepository> repo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<PayrollRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(PayrollRepoTest, InsertPayrollCalculationResults_Succeeds)
{
    auto p = makePayroll();
    sqlite3_int64 id = repo->insertPayroll(p);
    EXPECT_GT(id, 0) << "Expected a valid rowid after insert";
}

TEST_F(PayrollRepoTest, GetById_ReturnsPayrollCalculationResultsIfExists)
{
    PayrollCalculationResults p = makePayroll();
    auto id = repo->insertPayroll(p); // this is the 21st
    ASSERT_GT(id, 0);
    std::optional<PayrollCalculationResults> log = repo->getById(id);
    ASSERT_TRUE(log.has_value());
    EXPECT_EQ(log.value().id, id);
}

TEST_F(PayrollRepoTest, GetById_ReturnsNulloptIfRecordDoesNotExist)
{
    auto result = repo->getById(99999);
    EXPECT_FALSE(result.has_value()) << "Expected nullopt for non-existent id";
}

TEST_F(PayrollRepoTest, GetAll_ReturnsAllPayrollRecords)
{
    auto vP1 = repo->getAll();
    ASSERT_TRUE(vP1.size() == 20);
    auto id = repo->insertPayroll(makePayroll());
    ASSERT_GT(id, 0);
    auto vP2 = repo->getAll();
    ASSERT_TRUE(vP2.size() == 21);
}

TEST_F(PayrollRepoTest, GetByEmployeeId_ReturnsCorrectPayrollRecords)
{
    std::string newId = "01-0099";
    int monthFirstHalf = 1;
    int monthSecondHalf = 2;
    repo->insertPayroll(makePayroll(newId, "2026 03", monthFirstHalf));
    repo->insertPayroll(makePayroll(newId, "2026 03", monthSecondHalf));
    repo->insertPayroll(makePayroll(newId, "2026 04", monthFirstHalf));
    repo->insertPayroll(makePayroll(newId, "2026 04", monthSecondHalf));
    auto vP = repo->getAllById(newId);
    ASSERT_TRUE(vP.size() == 4);
    for (auto &item : vP)
        ASSERT_TRUE(item.employeeId == newId);
}

TEST_F(PayrollRepoTest, GetByPayPeriod_ReturnsMatchingRecords)
{
    std::string newIdOne = "01-0099";
    std::string newIdTwo = "01-0100";
    int monthFirstHalf = 1;
    int monthSecondHalf = 2;
    repo->insertPayroll(makePayroll(newIdOne, "2026 03", monthFirstHalf));
    repo->insertPayroll(makePayroll(newIdOne, "2026 03", monthSecondHalf));
    repo->insertPayroll(makePayroll(newIdOne, "2026 04", monthFirstHalf));
    repo->insertPayroll(makePayroll(newIdOne, "2026 04", monthSecondHalf));
    repo->insertPayroll(makePayroll(newIdOne, "2026 05", monthSecondHalf));
    repo->insertPayroll(makePayroll(newIdTwo, "2026 03", monthFirstHalf));
    repo->insertPayroll(makePayroll(newIdTwo, "2026 03", monthSecondHalf));
    repo->insertPayroll(makePayroll(newIdTwo, "2026 04", monthFirstHalf));
    repo->insertPayroll(makePayroll(newIdTwo, "2026 04", monthSecondHalf));
    repo->insertPayroll(makePayroll(newIdTwo, "2026 05", monthSecondHalf));

    ASSERT_TRUE(repo->getPayrollByPeriod("2026 03").size() == 4);
    ASSERT_TRUE(repo->getPayrollByPeriod("2026 03", newIdOne).size() == 2);
    ASSERT_TRUE(repo->getPayrollByPeriod("2026 03", std::nullopt, 1).size() == 2);
    ASSERT_TRUE(repo->getPayrollByPeriod("2026 03", newIdOne, 1).size() == 1);
    ASSERT_TRUE(repo->getPayrollByPeriod("2026 05").size() == 2);
    ASSERT_TRUE(repo->getPayrollByPeriod("2026 06").size() == 0);
}

TEST_F(PayrollRepoTest, UpdatePayrollCalculationResults_ModifiesExistingRecord)
{
    auto p = makePayroll();
    auto id = repo->insertPayroll(p);
    ASSERT_GT(id, 0);
    p.id = id;
    PayrollCalculationResults pc = p;
    pc.fullName = "bob santos";
    ASSERT_TRUE(repo->updatePayroll(pc));
    auto updated = repo->getById(id);
    ASSERT_TRUE(updated.has_value());
    ASSERT_TRUE(p.fullName != updated.value().fullName);
}

TEST_F(PayrollRepoTest, DeletePayrollCalculationResults_RemovesRecord)
{

    auto p = makePayroll();
    auto id = repo->insertPayroll(p);
    ASSERT_GT(id, 0);
    ASSERT_TRUE(repo->deletePayroll(id));
    ASSERT_FALSE(repo->getById(id).has_value());
}

TEST_F(PayrollRepoTest, CalculateGrossIncome_CorrectlySumsSalaryAllowanceAndOvertime)
{
    auto p = makePayroll();
    auto id = repo->insertPayroll(p);
    auto r = repo->getById(id);
    ASSERT_TRUE(r.has_value());

    double expected = p.monthlyBasicSalary + p.monthlyAllowances + p.overTimePay;
    std::cout << " p.monthlyBasicSalary: " << p.monthlyBasicSalary << ", p.monthlyAllowances: " << p.monthlyAllowances << ", p.overTimePay: " << p.overTimePay << ", expected: " << expected << ", r->grossIncome: " << r->grossIncome << std::endl;
    EXPECT_DOUBLE_EQ(r->grossIncome, expected);
}

TEST_F(PayrollRepoTest, CalculateTotalDeductions_SumsAllApplicableDeductions)
{
    auto p = makePayroll();
    auto id = repo->insertPayroll(p);
    auto r = repo->getById(static_cast<int>(id));
    ASSERT_TRUE(r.has_value());

    double expected = p.sssPremium_EE + p.philHealthPremium_EE + p.hdmfPremium_EE + p.withHoldingTax + p.loanDeductionsPerPayroll;
    std::cout
        << " p.sssPremium_EE: " << p.sssPremium_EE
        << ", p.philHealthPremium_EE: " << p.philHealthPremium_EE
        << ", p.hdmfPremium_EE: " << p.hdmfPremium_EE
        << ", p.withHoldingTax: " << p.withHoldingTax
        << ", p.loanDeductionsPerPayroll: " << p.loanDeductionsPerPayroll
        << ", expected: " << expected
        << ", r->totalDeductions: " << r->totalDeductions << std::endl;
    EXPECT_NEAR(r->totalDeductions, expected, 0.1);
}

TEST_F(PayrollRepoTest, NetPay_ComputesCorrectlyFromGrossAndDeductions)
{
    auto p = makePayroll();
    auto id = repo->insertPayroll(p);
    auto r = repo->getById(static_cast<int>(id));
    ASSERT_TRUE(r.has_value());

    double expected = p.grossIncome - p.totalDeductions;
    std::cout
        << ", expected: " << expected
        << ", r->netPay" << r->netPay
        << std::endl;
    EXPECT_NEAR(r->netPay, expected, 0.01);
}

TEST_F(PayrollRepoTest, SaveAndLoadConfig_RoundTripsCorrectly)
{
    PayrollConfig config{};
    config.sssSchedule = DeductionSchedule::FirstHalf;
    config.philHealthSchedule = DeductionSchedule::SecondHalf;
    config.hdmfSchedule = DeductionSchedule::BothHalves;

    ASSERT_TRUE(repo->saveConfig(config));

    auto loaded = repo->loadConfig();
    ASSERT_TRUE(loaded.has_value());
    EXPECT_EQ(loaded->sssSchedule, DeductionSchedule::FirstHalf);
    EXPECT_EQ(loaded->philHealthSchedule, DeductionSchedule::SecondHalf);
    EXPECT_EQ(loaded->hdmfSchedule, DeductionSchedule::BothHalves);
}

TEST_F(PayrollRepoTest, HandlesZeroAdjustmentsWithoutErrors)
{
    auto p = makePayroll();
    p.adjustments = 0.0;
    p.overTimePay = 0.0;

    auto id = repo->insertPayroll(p);
    EXPECT_GT(id, 0);

    auto r = repo->getById(static_cast<int>(id));
    ASSERT_TRUE(r.has_value());
    EXPECT_DOUBLE_EQ(r->adjustments, 0.0);
    EXPECT_DOUBLE_EQ(r->overTimePay, 0.0);
}

TEST_F(PayrollRepoTest, HandlesMultipleEmployeesForSamePayPeriod)
{

    // repo already has 20 elements via sql file
    repo->insertPayroll(makePayroll("01-0098", "2026 03", 1));
    repo->insertPayroll(makePayroll("01-0099", "2026 03", 1));
    repo->insertPayroll(makePayroll("01-0100", "2026 03", 1));

    auto all = repo->getAll();
    EXPECT_EQ(all.size(), 23u);

    std::cout
        << ", employeeId 1: " << all[20].employeeId
        << ", employeeId 2: " << all[21].employeeId
        << ", employeeId 3: " << all[22].employeeId
        << std::endl;

    // each should be a different employee
    EXPECT_EQ(all[20].employeeId, "01-0098");
    EXPECT_EQ(all[21].employeeId, "01-0099");
    EXPECT_EQ(all[22].employeeId, "01-0100");
}

TEST_F(PayrollRepoTest, PreventsDuplicatePayrollForSameEmployeeAndPayPeriod)
{
    auto newId1 = repo->insertPayroll(makePayroll("00-0099", "2026 07", 1));
    EXPECT_EQ(newId1, 21);

    auto newId2 = repo->insertPayroll(makePayroll("00-0099", "2026 07", 1));
    EXPECT_EQ(newId2, 0); // returns zero becuase its a duplicate

    auto result = repo->getPayrollByPeriod("2026 07", "00-0099", 1);
    ASSERT_TRUE(result.size() == 1);

    std::string empId = "00-0099";
    auto all = repo->getAllById(empId);
    EXPECT_EQ(all.size(), 1u) << "Duplicate payroll record was inserted";
}

TEST_F(PayrollRepoTest, Processed_IsStoredAndRetrievedCorrectly)
{
    auto p = makePayroll();
    p.dateProcessed = Date(2027, 12, 31);

    auto id = repo->insertPayroll(p);
    auto r = repo->getById(static_cast<int>(id));
    ASSERT_TRUE(r.has_value());

    EXPECT_EQ(r->dateProcessed.year, 2027);
    EXPECT_EQ(r->dateProcessed.month, 12);
    EXPECT_EQ(r->dateProcessed.day, 31);
}