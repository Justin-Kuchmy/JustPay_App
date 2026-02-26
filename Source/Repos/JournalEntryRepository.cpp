
#include "Repositories/JournalEntryRepository.h"

JournalEntryRepository::JournalEntryRepository(sqlite3 *db) : BaseRepository(db)
{
    LOG_DEBUG("\n JournalEntryRepository created");
}

bool JournalEntryRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/journalentry.sql");
}

JournalEntry JournalEntryRepository::mapJournalEntry(sqlite3_stmt *stmt)
{
    return {};
}

// create
sqlite3_int64 JournalEntryRepository::insertJournalEntry(const JournalEntry &journalEntry)
{
    return {};
}

// read
std::vector<JournalEntry> JournalEntryRepository::getAll()
{
    return {};
}
std::optional<JournalEntry> JournalEntryRepository::getById(int id)
{
    return std::nullopt;
}

// update
bool JournalEntryRepository::updateJournalEntry(const JournalEntry &lled)
{
    return false;
}

// delete
bool JournalEntryRepository::deleteJournalEntry(int id)
{
    return false;
}
std::string JournalEntryRepository::getLastJournalEntryId()
{
    return "";
}
