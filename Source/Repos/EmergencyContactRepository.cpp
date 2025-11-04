#include "Repositories/EmergencyContactRepository.h"




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
    c.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    c.relation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    c.address = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    c.contactNo = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));

    return c;
};



//CREATE
bool EmergencyContactRepository::insertContact(const Contact& Contact)
{
    LOG_DEBUG("EmergencyContactRepository::insertContact not implemented");
    return false;
};


// READ
std::optional<Contact> EmergencyContactRepository::getById(std::string id)
{
    LOG_DEBUG("EmergencyContactRepository::getById not implemented");
    return std::nullopt;
};
         

// UPDATE
bool EmergencyContactRepository::updateContact(const Contact& emp)
{
    LOG_DEBUG("EmergencyContactRepository::updateContact not implemented");
    return false;
}; 


// DELETE
bool EmergencyContactRepository::deleteContact(std::string id)
{
    LOG_DEBUG("EmergencyContactRepository::deleteContact not implemented");
    return false;
};  
      
std::string EmergencyContactRepository::getLastContactId()
{
   LOG_DEBUG("EmergencyContactRepository::deleteContact not getLastContactId");
   return "";
};

