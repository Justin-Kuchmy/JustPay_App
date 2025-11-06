#include "Repositories/DependentRepository.h"
#include <format>


DependentRepository::DependentRepository(sqlite3* db): BaseRepository(db)
{
    std::cout << "\n DependentRepository created" << std::endl;
};

std::string DependentRepository::getCreateTableSQL()const
{
    return R"(
        PRAGMA foreign_keys = OFF;
        DROP TABLE IF EXISTS dependents;

        CREATE TABLE IF NOT EXISTS dependents (
            dependentId INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            relation TEXT NOT NULL,
            birthday TEXT NOT NULL

        );

        -- DEPENDENTS (subset for realism)
        INSERT INTO dependents (name, relation, birthday) VALUES
        ('Lara Mendoza', 'Daughter', '2016-03-14'),
        ('Ethan Cruz', 'Son', '2018-09-21'),
        ('Sofia Reyes', 'Spouse', '1992-07-10'),
        ('Jacob Tan', 'Son', '2013-11-05'),
        ('Angela Lim', 'Mother', '1964-01-28'),
        ('Miguel Torres', 'Father', '1960-06-19'),
        ('Chloe Navarro', 'Daughter', '2017-12-02'),
        ('Lucas Ramos', 'Son', '2015-05-09'),
        ('Bianca Uy', 'Spouse', '1990-10-15'),
        ('Daniel Bautista', 'Brother', '1995-04-04');

    )";
};

Dependent DependentRepository::mapDependent(sqlite3_stmt* stmt)
{
    Dependent d;
    d.dependentId = sqlite3_column_int(stmt, 0);
    d.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    d.relation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    const unsigned char* text = sqlite3_column_text(stmt, 3);
    if (text) {
        d.birthday = from_string(reinterpret_cast<const char*>(text));
    }
    else {
        d.birthday = Date{1900, 1, 1};
    }
    return d;
};


//CREATE
int DependentRepository::insertDependent(const Dependent& dependent)
{
    const char* sql = "INSERT INTO dependents (name, relation, birthday) VALUES (?,?,?)";
    sqlite3_stmt* stmt = nullptr;
    int result = 0;
    //database, sql_statement, max_length, out_statement, ptr to unused part of sql string
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0;  // Failed to prepare


    // Binds a text value to the 1st parameter (the first ? placeholder) in the prepared statement.
    // Params:
    //   stmt            → The prepared sqlite3_stmt* returned by sqlite3_prepare_v2()
    //   1               → The parameter index to bind (1-based, not 0-based)
    //   Dependent.namc.c_str() → Pointer to the null-terminated C-string containing the text to bind
    //   -1              → Length of the text; -1 tells SQLite to read until the null terminator
    //   SQLITE_TRANSIENT → Instructs SQLite to make its own copy of the string (safe if original may go out of scope)
    sqlite3_bind_text(stmt, 1, dependent.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dependent.relation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, to_string(dependent.birthday).c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
        return 0;  // Insert failed
    return sqlite3_last_insert_rowid(db);
};

// READ
std::optional<Dependent> DependentRepository::getById(int dependentId)
{
    const char* sql = "SELECT * from dependents where dependentId == ?";
    sqlite3_stmt* stmt = nullptr;
    std::optional<Dependent> result = std::nullopt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return result;
    }
    else
        sqlite3_bind_int(stmt, 1, dependentId);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapDependent(stmt);
    }
    sqlite3_finalize(stmt);
    return result;
};             

// UPDATE
bool DependentRepository::updateDependent(const Dependent& d)
{
    std::string sql = std::format("update Dependents set name='{}', relation='{}', birthday='{}' where dependentId = '{}'",
        d.name,
        d.relation,
        to_string(d.birthday),
        d.dependentId
        );
    return DependentRepository::execute(sql);
}; 

// DELETE
bool DependentRepository::deleteDependent(int id)
{
    std::string sql = std::format("DELETE from dependents where dependentId = '{}'",id);
    return DependentRepository::execute(sql);
};


std::string DependentRepository::getLastDependentId()
{    LOG_DEBUG("DependentRepository::getLastDependentId not implemented");
    return "";

};
