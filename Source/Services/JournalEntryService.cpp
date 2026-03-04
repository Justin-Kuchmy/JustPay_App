
#include "Services/JournalEntryService.h"

JournalEntryService::JournalEntryService(JournalEntryRepository &r) : repo(r)
{
}

// CREATE
sqlite3_int64 JournalEntryService::addJournalEntry(const JournalEntry &JournalEntry)
{
    return {};
}

bool JournalEntryService::addJournalEntries(const std::vector<JournalEntry> &entries)
{
    return false;
}
// READ
std::optional<JournalEntry> JournalEntryService::getJournalEntryByID(int id) const
{
    return {};
}

std::vector<JournalEntry> JournalEntryService::getAll() const
{
    return this->repo.getAll();
}

// UPDATE
bool JournalEntryService::updateJournalEntry(const JournalEntry &e)
{
    return false;
}

// DELETE
bool JournalEntryService::deleteJournalEntry(int id)
{
    return false;
}