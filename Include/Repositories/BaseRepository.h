#ifndef BASEREPO_H
#define BASEREPO_H

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <optional>
#include <functional>

class BaseRepository {
protected: 
    sqlite3* db = nullptr;
public:
    explicit BaseRepository(sqlite3* db);
    virtual ~BaseRepository();
    virtual std::string getCreateTableSQL() const = 0;
    bool createTable();
    
protected:
    bool execute(const std::string& sql);
    template <typename T>
    std::vector<T> query(const std::string& sql,std::function<T(sqlite3_stmt*)> mapper)
    {
        std::vector<T> results;
        sqlite3_stmt* stmt; /* OUT: Statement handle */

        
        //database, sql_statement, max_length, out_statement, ptr to unused part of sql string
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
            return results;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            results.push_back(mapper(stmt));
        }

        sqlite3_finalize(stmt);
        return results;
    }
};


#endif