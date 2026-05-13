#include "Repositories/DependentRepository.h"
#include <format>

DependentRepository::DependentRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\n DependentRepository created" << std::endl;
};
bool DependentRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/dependent.sql");
};

bool DependentRepository::exists(const std::string &name, const Date &birthday)
{
    std::string sql = "SELECT * FROM dependents WHERE name = ? AND birthday = ?";
    auto results = this->query<Dependent>(sql, mapDependent, [&name, &birthday](sqlite3_stmt *stmt)
                                          {
        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, birthday.to_string().c_str(), -1, SQLITE_TRANSIENT); });
    return !results.empty();
}

Dependent DependentRepository::mapDependent(sqlite3_stmt *stmt)
{
    Dependent d;
    d.dependentId = sqlite3_column_int(stmt, 0);
    d.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    d.relation = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    const unsigned char *text = sqlite3_column_text(stmt, 3);
    if (text)
    {
        d.birthday = Date::fromString(reinterpret_cast<const char *>(text));
    }
    else
    {
        d.birthday = Date{1900, 1, 1};
    }
    return d;
};

void DependentRepository::bindDependent(sqlite3_stmt *stmt, const Dependent &d)
{
    int column{1};
    sqlite3_bind_text(stmt, column++, d.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, d.relation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, d.birthday.to_string().c_str(), -1, SQLITE_TRANSIENT);
}

// CREATE
sqlite3_int64 DependentRepository::insertDependent(const Dependent &dependent)
{
    const char *sql = "INSERT INTO dependents (name, relation, birthday) VALUES (?,?,?)";
    sqlite3_stmt *stmt = nullptr;
    // database, sql_statement, max_length, out_statement, ptr to unused part of sql string
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0; // Failed to prepare

    // Binds a text value to the 1st parameter (the first ? placeholder) in the prepared statement.
    // Params:
    //   stmt            → The prepared sqlite3_stmt* returned by sqlite3_prepare_v2()
    //   1               → The parameter index to bind (1-based, not 0-based)
    //   Dependent.namc.c_str() → Pointer to the null-terminated C-string containing the text to bind
    //   -1              → Length of the text; -1 tells SQLite to read until the null terminator
    //   SQLITE_TRANSIENT → Instructs SQLite to make its own copy of the string (safe if original may go out of scope)
    sqlite3_bind_text(stmt, 1, dependent.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, dependent.relation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dependent.birthday.to_string().c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
        return 0; // Insert failed
    return sqlite3_last_insert_rowid(db);
};

// READ
std::optional<Dependent> DependentRepository::getById(int dependentId)
{
    const char *sql = "SELECT * from dependents where dependentId == ?";
    sqlite3_stmt *stmt = nullptr;
    std::optional<Dependent> result = std::nullopt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
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
bool DependentRepository::updateDependent(const Dependent &d)
{
    std::string sql = "UPDATE dependents SET name=?, relation=?, birthday=? WHERE dependentId = ?";
    return DependentRepository::execute(sql, [&d](sqlite3_stmt *stmt)
                                        {
        bindDependent(stmt, d);
        sqlite3_bind_int(stmt, 4, d.dependentId); });
}

// DELETE
bool DependentRepository::deleteDependent(int id)
{
    std::string sql = "DELETE FROM dependents WHERE dependentId = ?";
    return DependentRepository::execute(sql, [&id](sqlite3_stmt *stmt)
                                        { sqlite3_bind_int(stmt, 1, id); });
}

std::string DependentRepository::getLastDependentId()
{
    LOG_DEBUG("DependentRepository::getLastDependentId not implemented");
    return "";
};
