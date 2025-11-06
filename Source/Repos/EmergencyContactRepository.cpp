#include "Repositories/EmergencyContactRepository.h"
#include <format>




EmergencyContactRepository::EmergencyContactRepository(sqlite3* db): BaseRepository(db)
{
    std::cout << "\n EmergencyContactRepository created" << std::endl;
};

std::string EmergencyContactRepository::getCreateTableSQL()const
{
    LOG_DEBUG("EmergencyContactRepository::getCreateTableSQL()");
    return R"(
        PRAGMA foreign_keys = OFF;
        DROP TABLE IF EXISTS emergency_contacts;

        CREATE TABLE IF NOT EXISTS emergency_contacts (
            contactId INTEGER PRIMARY KEY AUTOINCREMENT,
            employeeId TEXT NOT NULL,
            name TEXT,
            relation TEXT,
            address TEXT,
            contactNo TEXT,
            FOREIGN KEY (employeeId) REFERENCES employees(employeeId)
        );


        -- EMERGENCY CONTACTS (sampled)
        INSERT INTO "emergency_contacts" (employeeId, name, relation, address, contactNo) VALUES
        ('00-0001','Juan Santos','Brother','Quezon City','09171234567'),
        ('00-0016','Andrea Santos','Mother','Cavite','09191231231'),
        ('00-0038','Wendy Santos','Sister','Makati','09181115555'),
        ('01-0002','Maria Reyes','Mother','Makati','09987654321'),
        ('01-0018','Jose Bautista','Father','Pasay','09281113333'),
        ('01-0040','Xavier Lim','Brother','Cebu City','09193334444'),
        ('02-0004','Andrea Dela Cruz','Wife','Pasig','09181112222'),
        ('02-0020','Nicole Ramos','Sister','Taguig','09191230000'),
        ('02-0035','Sofia Tan','Sister','Antipolo','09399990000'),
        ('03-0006','Camille Gomez','Sister','Taguig','09170001111'),
        ('03-0037','Ulyssa Dizon','Friend','Pasay','09181239876'),
        ('04-0008','Lorenzo Co','Father','Caloocan','09285551234'),
        ('05-0010','Karen Ramos','Mother','Mandaluyong','09453334444'),
        ('05-0026','Kelly Ramos','Sister','Las Piñas','09175556666'),
        ('06-0012','Carlo Robles','Husband','Pasig','09285557777'),
        ('07-0014','Allan Cruz','Brother','Manila','09187775555'),
        ('08-0015','Cheryl Chua','Wife','Quezon City','09356669999');
    )";
};

Contact EmergencyContactRepository::mapContact(sqlite3_stmt* stmt)
{
    Contact c;
    c.contactId = sqlite3_column_int(stmt, 0);
    c.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    c.relation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    c.address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    c.contactNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

    return c;
};



//CREATE
int EmergencyContactRepository::insertContact(const Contact& contact)
{
    const char* sql = "INSERT INTO emergency_contacts (name, relation, address, contactNo) VALUES (?,?,?,?)";
    sqlite3_stmt* stmt = nullptr;
    int result = 0;
    //database, sql_statement, max_length, out_statement, ptr to unused part of sql string
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        return 0;  // Failed to prepare


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
        return 0;  // Insert failed
    return sqlite3_last_insert_rowid(db);
};


// READ
std::optional<Contact> EmergencyContactRepository::getById(int contactId)
{
    const char* sql = "SELECT * from emergency_contacts where contactId == ?";
    sqlite3_stmt* stmt = nullptr;
    std::optional<Contact> result = std::nullopt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
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
bool EmergencyContactRepository::updateContact(const Contact& c)
{
        std::string sql = std::format("update emergency_contacts set name='{}', relation='{}', address='{}', contactNo='{}' where contactId = '{}'",
        c.name,
        c.relation,
        c.address,
        c.contactNo,
        c.contactId
        );
    return EmergencyContactRepository::execute(sql);
}; 


// DELETE
bool EmergencyContactRepository::deleteContact(int id)
{
    std::string sql = std::format("DELETE from emergency_contacts where contactId = '{}'",id);
    return EmergencyContactRepository::execute(sql);
};  
      
int EmergencyContactRepository::getLastContactId()
{
   return 0;
};

