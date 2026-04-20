#include "Repositories/LoanLedgerRepository.h"
#include <format>
#define DEBUG_LOGS
#include "Utils/Log.h"

LoanLedgerRepository::LoanLedgerRepository(sqlite3 *db) : BaseRepository(db)
{
    LOG_DEBUG("\n LoanLedgerRepository created");
};
bool LoanLedgerRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/loanledger.sql");
};

void LoanLedgerRepository::bindLoanLedger(sqlite3_stmt *stmt, const LoanLedger &lled)
{
    int column{1};
    sqlite3_bind_int(stmt, column++, static_cast<int>(lled.loanType));
    sqlite3_bind_double(stmt, column++, lled.principalAmount);
    sqlite3_bind_text(stmt, column++, lled.loanDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, column++, lled.NumOfAmortizations);
    sqlite3_bind_double(stmt, column++, lled.deductionsPerPayroll);
    sqlite3_bind_int(stmt, column++, lled.deductionFirstHalf);
    sqlite3_bind_int(stmt, column++, lled.deductionSecondHalf);
    sqlite3_bind_int(stmt, column++, lled.status);
    sqlite3_bind_text(stmt, column++, lled.employeeId.c_str(), -1, SQLITE_TRANSIENT);
}
LoanLedger LoanLedgerRepository::mapLoanLedger(sqlite3_stmt *stmt)
{
    LoanLedger lled;
    lled.loanLedgerId = sqlite3_column_int(stmt, 0);
    const unsigned char *empid_text = sqlite3_column_text(stmt, 1);
    lled.employeeId = empid_text ? reinterpret_cast<const char *>(empid_text) : std::string{};
    lled.loanType = static_cast<LoanType>(sqlite3_column_int(stmt, 2));
    lled.principalAmount = sqlite3_column_double(stmt, 3);
    const unsigned char *date = sqlite3_column_text(stmt, 4);
    if (date)
    {
        lled.loanDate = Date::fromString(reinterpret_cast<const char *>(date));
    }
    else
    {
        lled.loanDate = Date(1900, 1, 1);
    }

    lled.NumOfAmortizations = sqlite3_column_int(stmt, 5);
    lled.deductionsPerPayroll = sqlite3_column_double(stmt, 6);
    lled.deductionFirstHalf = sqlite3_column_int(stmt, 7);  // converts to true or false
    lled.deductionSecondHalf = sqlite3_column_int(stmt, 8); // converts to true or false
    lled.status = sqlite3_column_int(stmt, 9);
    return lled;
};

// create
sqlite3_int64 LoanLedgerRepository::insertLoanLedger(const LoanLedger &lled)
{
    const char *sql = R"(INSERT INTO loan_ledgers (
        loanType,
        principalAmount,
        loanDate,
        NumOfAmortizations,
        deductionsPerPayroll,
        deductionFirstHalf,
        deductionSecondHalf,
        status,
        employeeId
        ) VALUES (?,?,?,?,?,?,?,?,?)
    )";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("SQL prepare failed:" << sqlite3_errmsg(db));
        return 0;
    }
    LoanLedgerRepository::bindLoanLedger(stmt, lled);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)

    {
        LOG_DEBUG("SQL insert failed:" << sqlite3_errmsg(db));
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
};

// read
std::optional<LoanLedger> LoanLedgerRepository::getById(int id)
{
    const char *sql = "SELECT * from loan_ledgers where loanLedgerId = ?";
    sqlite3_stmt *stmt = nullptr;
    std::optional<LoanLedger> result = std::nullopt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return result;
    }
    else
        sqlite3_bind_int(stmt, 1, id);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapLoanLedger(stmt);
        LOG_DEBUG("ID: " << result.value().loanLedgerId);
    }
    sqlite3_finalize(stmt);
    return result;
};

std::vector<LoanLedger> LoanLedgerRepository::getAllById(std::string id)
{
    std::string sql = std::format("SELECT * from loan_ledgers where employeeId = '{}'", id);

    auto results = this->query<LoanLedger>(sql, mapLoanLedger);

    if (results.size() > 0)
    {
        return results;
    }
    else
    {
        // no loans for this employee
        return {};
    }
};

// update
bool LoanLedgerRepository::updateLoanLedger(const LoanLedger &lled)
{

    std::string sql = R"(
        UPDATE loan_ledgers SET 
        loanType = ?,
        principalAmount = ?,
        loanDate = ?,
        NumOfAmortizations = ?,
        deductionsPerPayroll = ?,
        deductionFirstHalf = ?,
        deductionSecondHalf = ?,
        status = ?,
        employeeId = ?
        WHERE loanLedgerId = ?
    )";
    return execute(sql, [&lled](sqlite3_stmt *stmt)
                   { 
                    bindLoanLedger(stmt, lled); 
                    sqlite3_bind_int(stmt, 10, lled.loanLedgerId); });
};

// delete
bool LoanLedgerRepository::deleteLoanLedger(int id)
{
    std::string sql = R"(
        DELETE
        FROM loan_ledgers
        WHERE loanLedgerId = ?
    )";
    return execute(sql, [&id](sqlite3_stmt *stmt)
                   { sqlite3_bind_int(stmt, 1, id); });
};
