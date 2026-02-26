#ifndef JOURNALENTRYSERVICE_H
#define JOURNALENTRYSERVICE_H
#include "Repositories/JournalEntryRepository.h"
#include "../Models/DataObjects.h"
#include <optional>
#include <vector>

class JournalEntryService
{
private:
    JournalEntryRepository &repo;

public:
    explicit JournalEntryService(JournalEntryRepository &r);

    // CREATE
    sqlite3_int64 addJournalEntry(const JournalEntry &journalEntry);

    bool addJournalEntries(const std::vector<JournalEntry> &entries);

    // READ
    std::optional<JournalEntry> getJournalEntryByID(int id) const;

    std::vector<JournalEntry> getAll() const;

    // UPDATE
    bool updateJournalEntry(const JournalEntry &e);

    // DELETE
    bool deleteJournalEntry(int id);
};

#endif