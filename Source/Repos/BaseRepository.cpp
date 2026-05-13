#include "Repositories/BaseRepository.h"
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
        return false;
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