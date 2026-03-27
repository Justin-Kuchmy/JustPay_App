#ifndef BASEREPO_H
#define BASEREPO_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <optional>
#include <functional>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include <QFile>
#include <QTextStream>

class BaseRepository
{
protected:
    sqlite3 *db = nullptr;

public:
    explicit BaseRepository(sqlite3 *db);
    BaseRepository(const BaseRepository &) = delete;
    BaseRepository &operator=(const BaseRepository &) = delete;
    virtual ~BaseRepository();
    virtual bool createTable() const = 0;

protected:
    bool execute(const std::string &sql, std::function<void(sqlite3_stmt *)> binder = [](sqlite3_stmt *) {}) const;
    bool executeFile(const QString &filePath) const;
    template <typename T>
    std::vector<T> query(const std::string &sql, std::function<T(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder = [](sqlite3_stmt *) {}) const
    {
        std::vector<T> results;
        sqlite3_stmt *stmt; /* OUT: Statement handle */

        // database, sql_statement, max_length, out_statement, ptr to unused part of sql string
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
            return results;
        }
        binder(stmt);
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            results.push_back(mapper(stmt));
        }

        sqlite3_finalize(stmt);
        return results;
    }

    template <typename T>
    std::optional<T> querySingle(const std::string &sql, std::function<T(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder = [](sqlite3_stmt *) {}) const
    {
        std::optional<T> result = std::nullopt;
        sqlite3_stmt *stmt = nullptr;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        {
            std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
            return result;
        }

        binder(stmt);
        if (sqlite3_step(stmt) == SQLITE_ROW)
            result = mapper(stmt);

        sqlite3_finalize(stmt);
        return result;
    }
};

#endif