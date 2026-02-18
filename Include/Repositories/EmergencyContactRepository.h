#ifndef EMERGENCYCONTACTREPO_H
#define EMERGENCYCONTACTREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class EmergencyContactRepository : public BaseRepository
{
    static Contact mapContact(sqlite3_stmt *stmt);

public:
    explicit EmergencyContactRepository(sqlite3 *db);

    bool createTable() const override;

    // CREATE
    sqlite3_int64 insertContact(const Contact &Contact);

    // READ
    std::optional<Contact> getById(int id);

    // UPDATE
    bool updateContact(const Contact &emp);

    // DELETE
    bool deleteContact(int id);
    int getLastContactId();
};

#endif