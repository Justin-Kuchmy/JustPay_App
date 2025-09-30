#ifndef BASEREPO_HPP
#define BASEREPO_HPP

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <vector>
#include <optional>

class BaseRepository {
protected:
    sqlite3* db = nullptr;
    
public:
    explicit BaseRepository(const std::string& dbName);
    virtual ~BaseRepository();
    virtual std::string getCreateTableSQL() const = 0;
    bool createTable();
    
protected:
    bool execute(const std::string& sql);
};


#endif