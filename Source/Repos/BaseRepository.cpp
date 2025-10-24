#include "Repositories/BaseRepository.h"
#include "Models/DataObjects.h"

BaseRepository::BaseRepository(sqlite3* db) : db(db) {
    qDebug() << "BaseRepository created with existing DB connection";
}

BaseRepository::~BaseRepository() {
    qDebug() << "BaseRepository destroyed";
}

bool BaseRepository::execute(const std::string& sql) {
    qDebug() <<"execute: " << QString::fromStdString(sql);
    char* errMsg = nullptr;
    int rc = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        qDebug() << "SQL error: " << errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

template std::vector<Employee> BaseRepository::query(const std::string& sql,std::function<Employee(sqlite3_stmt*)> mapper);      

bool BaseRepository::createTable() {
    qDebug() << "Created Table via BaseRepo";
    return execute(getCreateTableSQL());
}
