#ifndef JOURNALENTRYREPO_H
#define JOURNALENTRYREPO_H
#include "BaseRepository.h"
#include "Models/accounting.h"
#include <sqlite3.h>

class JournalEntryRepository : public BaseRepository
{
    static JournalEntry mapJournalEntry(sqlite3_stmt *stmt);

public:
    explicit JournalEntryRepository(sqlite3 *db);

    bool createTable() const override;

    // create
    sqlite3_int64 insertJournalEntry(const JournalEntry &journalEntry);

    // read
    std::vector<JournalEntry> getAll();
    std::optional<JournalEntry> getById(int id);

    // update
    bool updateJournalEntry(const JournalEntry &lled);

    // delete
    bool deleteJournalEntry(int id);
    std::string getLastJournalEntryId();
};

#endif