#include "Repositories/EmergencyContactRepository.h"

EmergencyContactRepository::EmergencyContactRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\n EmergencyContactRepository created" << std::endl;
};

bool EmergencyContactRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/emergencycontact.sql");
};

Contact EmergencyContactRepository::mapContact(sqlite3_stmt *stmt)
{
    Contact c;
    c.contactId = sqlite3_column_int(stmt, 0);
    c.name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    c.relation = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    c.address = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
    c.contactNo = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));

    return c;
};

void EmergencyContactRepository::bindContact(sqlite3_stmt *stmt, const Contact &c)
{
    int column{1};
    sqlite3_bind_text(stmt, column++, c.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, c.relation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, c.address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, c.contactNo.c_str(), -1, SQLITE_TRANSIENT);
}

// CREATE
sqlite3_int64 EmergencyContactRepository::insertContact(const Contact &contact)
{
    const char *sql = "INSERT INTO emergency_contacts (name, relation, address, contactNo) VALUES (?,?,?,?)";
    sqlite3_stmt *stmt = nullptr;
    // database, sql_statement, max_length, out_statement, ptr to unused part of sql string
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0; // Failed to prepare

    // Binds a text value to the 1st parameter (the first ? placeholder) in the prepared statement.
    // Params:
    //   stmt            → The prepared sqlite3_stmt* returned by sqlite3_prepare_v2()
    //   1               → The parameter index to bind (1-based, not 0-based)
    //   contact.namc.c_str() → Pointer to the null-terminated C-string containing the text to bind
    //   -1              → Length of the text; -1 tells SQLite to read until the null terminator
    //   SQLITE_TRANSIENT → Instructs SQLite to make its own copy of the string (safe if original may go out of scope)
    sqlite3_bind_text(stmt, 1, contact.name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, contact.relation.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, contact.address.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, contact.contactNo.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
        return 0; // Insert failed
    return sqlite3_last_insert_rowid(db);
};

// READ
std::optional<Contact> EmergencyContactRepository::getById(int contactId)
{
    const char *sql = "SELECT * from emergency_contacts where contactId == ?";
    sqlite3_stmt *stmt = nullptr;
    std::optional<Contact> result = std::nullopt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return result;
    }
    else
        sqlite3_bind_int(stmt, 1, contactId);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapContact(stmt);
    }
    sqlite3_finalize(stmt);
    return result;
};

// UPDATE
bool EmergencyContactRepository::updateContact(const Contact &c)
{
    std::string sql = "UPDATE emergency_contacts SET name=?, relation=?, address=?, contactNo=? WHERE contactId = ?";
    return EmergencyContactRepository::execute(sql, [&c](sqlite3_stmt *stmt)
                                               {
        bindContact(stmt, c);
        sqlite3_bind_int(stmt, 5, c.contactId); });
}

// DELETE
bool EmergencyContactRepository::deleteContact(int id)
{
    std::string sql = "DELETE FROM emergency_contacts WHERE contactId = ?";
    return EmergencyContactRepository::execute(sql, [&id](sqlite3_stmt *stmt)
                                               { sqlite3_bind_int(stmt, 1, id); });
}

int EmergencyContactRepository::getLastContactId()
{
    return 0;
};
