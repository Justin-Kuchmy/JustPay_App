#include "../../include/Repos/BaseRepository.h"
#include "../../include/Entities/DataObjects.h"

BaseRepository::BaseRepository(const std::string& dbName) {
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
    }
}

BaseRepository::~BaseRepository() {
    if (db) {
        sqlite3_close(db);
        std::cout << "DB closed" << std::endl;
    }
}

bool BaseRepository::execute(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

template std::vector<Employee> BaseRepository::query(const std::string& sql,std::function<Employee(sqlite3_stmt*)> mapper);      

bool BaseRepository::createTable() {
    return execute(getCreateTableSQL());
}
