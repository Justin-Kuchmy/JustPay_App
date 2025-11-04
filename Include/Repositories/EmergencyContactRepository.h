#ifndef EMERGENCYCONTACTREPO_H
#define EMERGENCYCONTACTREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class EmergencyContactRepository: public BaseRepository {
    static Contact mapContact(sqlite3_stmt* stmt);

    public:
        explicit EmergencyContactRepository(sqlite3* db);
        std::string getCreateTableSQL() const override;
        
        //CREATE
        bool insertContact(const Contact& Contact);

        // READ
        std::optional<Contact> getById(std::string id);            

        // UPDATE
        bool updateContact(const Contact& emp); 

        // DELETE
        bool deleteContact(std::string id);        
        std::string getLastContactId();

};

#endif