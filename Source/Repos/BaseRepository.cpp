#include "Repositories/BaseRepository.h"
#include "Models/DataObjects.h"
#include "Utils/Log.h"

BaseRepository::BaseRepository(sqlite3 *db) : db(db)
{
    std::cout << "\nBaseRepository created with existing DB connection";
}

BaseRepository::~BaseRepository()
{
    std::cout << "\nBaseRepository destroyed";
}

bool BaseRepository::execute(const std::string &sql, std::function<void(sqlite3_stmt *)> binder) const
{
    sqlite3_stmt *stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return false;
    }

    binder(stmt);

    int rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_DEBUG("Execution failed: " << sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return false;
    }
    int changes = sqlite3_changes(db);
    sqlite3_finalize(stmt);

    return changes > 0;
}

bool BaseRepository::executeFile(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_DEBUG("Failed to open file");
        return "";
    }
    QTextStream in(&file);
    const std::string sql = in.readAll().toStdString();
    file.close();

    char *errMsg = nullptr;
    int rc = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        LOG_DEBUG("SQL error: " << errMsg);
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

template std::vector<Employee> BaseRepository::query(const std::string &sql, std::function<Employee(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<Contact> BaseRepository::query(const std::string &sql, std::function<Contact(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<Dependent> BaseRepository::query(const std::string &sql, std::function<Dependent(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<LoanLedger> BaseRepository::query(const std::string &sql, std::function<LoanLedger(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<PayrollCalculationResults> BaseRepository::query(const std::string &sql, std::function<PayrollCalculationResults(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<JournalEntry> BaseRepository::query(const std::string &sql, std::function<JournalEntry(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<GovernmentRemittance> BaseRepository::query(const std::string &sql, std::function<GovernmentRemittance(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<BudgetPeriod> BaseRepository::query(const std::string &sql, std::function<BudgetPeriod(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::vector<DepartmentBudget> BaseRepository::query(const std::string &sql, std::function<DepartmentBudget(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;

template std::optional<BudgetPeriod> BaseRepository::querySingle(const std::string &sql, std::function<BudgetPeriod(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::optional<DepartmentBudget> BaseRepository::querySingle(const std::string &sql, std::function<DepartmentBudget(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;