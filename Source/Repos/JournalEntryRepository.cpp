
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
    JournalEntry jEntry;
    jEntry.entryId = sqlite3_column_int(stmt, 0);

    jEntry.accountType = static_cast<AccountType>(sqlite3_column_int(stmt, 1));

    const unsigned char *accountNameString = sqlite3_column_text(stmt, 2);
    jEntry.accountName = accountNameString ? reinterpret_cast<const char *>(accountNameString) : std::string{};

    jEntry.debit = sqlite3_column_double(stmt, 3);

    jEntry.credit = sqlite3_column_double(stmt, 4);

    const unsigned char *periodHalfString = sqlite3_column_text(stmt, 5);
    jEntry.periodHalf = periodHalfString ? reinterpret_cast<const char *>(periodHalfString) : std::string{};

    int periodTextValue = sqlite3_column_int(stmt, 6);
    jEntry.periodText = periodTextValue == 1 ? "First Half" : "Second Half";

    return jEntry;
}

// create
sqlite3_int64 JournalEntryRepository::insertJournalEntry(const JournalEntry &journalEntry)
{
    const char *sql = R"(INSERT INTO journal_entries (
            accountType, 
            accountName, 
            debit, 
            credit, 
            pay_period_text, 
            pay_period_half
        ) values (?,?,?,?,?,?,)
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("SQL prepare failed: " << sqlite3_errmsg(db));
    }

    int idx = 1;
    sqlite3_bind_int(stmt, idx++, journalEntry.entryId);
    sqlite3_bind_text(stmt, idx++, journalEntry.accountName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(journalEntry.accountType));
    sqlite3_bind_double(stmt, idx++, journalEntry.debit);
    sqlite3_bind_double(stmt, idx++, journalEntry.credit);
    sqlite3_bind_text(stmt, idx++, journalEntry.periodText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, journalEntry.periodHalf.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_DEBUG("SQL insert failed: " << sqlite3_errmsg(db));
        return 0;
    }

    return sqlite3_last_insert_rowid(db);
}

// read
std::vector<JournalEntry> JournalEntryRepository::getAll()
{
    std::string sql = std::format("SELECT * FROM journal_entries");

    auto results = this->query<JournalEntry>(sql, mapJournalEntry);
    if (!results.empty())
    {
        return results;
    }
    else
    {
        LOG_DEBUG("Failed to get journal entries from the db");
        return {};
    }
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
