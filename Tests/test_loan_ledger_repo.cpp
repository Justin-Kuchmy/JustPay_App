#include <gtest/gtest.h>
#include "Repositories/LoanLedgerRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"



static void runSqlScript(sqlite3* db, const std::string& sqlFilePath)
{
    std::ifstream file(sqlFilePath);
    ASSERT_TRUE(file.is_open()) << "Failed to open SQL file: " << sqlFilePath;
    std::string sql((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    ASSERT_EQ(rc, SQLITE_OK) << "SQL error: " << (errMsg ? errMsg : "");
    if (errMsg) sqlite3_free(errMsg);
};

class LoanLedgerTest: public ::testing::Test {
protected:
    sqlite3* db = nullptr;
    std::unique_ptr<LoanLedgerRepository> lledRepo;

    void SetUp() override 
    {
        const char* dbName = "tests.db";
        sqlite3_open(dbName, &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db,sqlPath.string());

        lledRepo = std::make_unique<LoanLedgerRepository>(db);
    }

    void TearDown() override 
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(LoanLedgerTest, GetById_ReturnsLoanLedgerIfExists)
{
    auto loanLedger = lledRepo->getById(2);
    ASSERT_TRUE(loanLedger.has_value());
    EXPECT_EQ(loanLedger->principalAmount, 25000.00);
}

TEST_F(LoanLedgerTest, GetAllById_ReturnsLoanLedgerIfExists)
{
    std::vector<LoanLedger> loanLedger = lledRepo->getAllById("00-0001");
    ASSERT_TRUE(loanLedger.size());
    EXPECT_EQ(loanLedger.size(), 2);
}

TEST_F(LoanLedgerTest, InsertLoanLedger_ReturnsLoanLedgerID)
{
    LoanLedger lled;
    lled.loanType = LoanType::Personal_Cash_Advance;
    lled.principalAmount = 5000.50;
    lled.loanDate = Date{2025,11,11};
    lled.NumOfAmortizations = 25;
    lled.deductionsPerPayroll = (lled.principalAmount/lled.NumOfAmortizations);
    lled.deductionFirstHalf = true;
    lled.deductionSecondHalf = true;
    lled.status = "processed";
    lled.employeeId = "00-0001";

    int result = lledRepo->insertLoanLedger(lled);
    std::cout << result << std::endl;
    ASSERT_TRUE(result > 0);
    EXPECT_EQ(result, 3);
}

TEST_F(LoanLedgerTest, UpdateLoanLedger_ReturnsTrue)
{
    std::optional<LoanLedger> loanLedger = lledRepo->getById(1);
    ASSERT_TRUE(loanLedger.has_value()) << "No loan ledger found for 1";
    auto result = loanLedger.value();
    result.principalAmount = 33333;
    std::cout << "loanLedgerId: "<< result.loanLedgerId << std::endl;
    ASSERT_TRUE(lledRepo->updateLoanLedger(result));
    EXPECT_EQ(lledRepo->getById(1).value().principalAmount, 33333);

}

TEST_F(LoanLedgerTest, DeleteLoanLedger_ReturnsTrue)
{
   std::optional<LoanLedger> lled = lledRepo->getById(2);
    ASSERT_TRUE(lled.has_value());
    ASSERT_TRUE(lledRepo->deleteLoanLedger(lled.value().loanLedgerId));
    ASSERT_FALSE(lledRepo->getById(2).has_value());
}