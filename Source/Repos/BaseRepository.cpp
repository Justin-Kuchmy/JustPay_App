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

bool BaseRepository::execute(const std::string &sql) const
{
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

template std::vector<Employee> BaseRepository::query(const std::string &sql, std::function<Employee(sqlite3_stmt *)> mapper);
template std::vector<Contact> BaseRepository::query(const std::string &sql, std::function<Contact(sqlite3_stmt *)> mapper);
template std::vector<Dependent> BaseRepository::query(const std::string &sql, std::function<Dependent(sqlite3_stmt *)> mapper);
template std::vector<LoanLedger> BaseRepository::query(const std::string &sql, std::function<LoanLedger(sqlite3_stmt *)> mapper);
template std::vector<PayrollCalculationResults> BaseRepository::query(const std::string &sql, std::function<PayrollCalculationResults(sqlite3_stmt *)> mapper);
