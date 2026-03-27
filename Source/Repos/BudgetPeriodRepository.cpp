#include "Repositories/BudgetPeriodRepository.h"

BudgetPeriod BudgetPeriodRepository::mapBudgetPeriod(sqlite3_stmt *stmt)
{
    BudgetPeriod bp;
    bp.id = sqlite3_column_int(stmt, 0);
    bp.label = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    bp.year = sqlite3_column_int(stmt, 2);
    bp.half = sqlite3_column_int(stmt, 3);
    const unsigned char *start = sqlite3_column_text(stmt, 4);
    if (start)
        bp.startDate = Date::fromString(reinterpret_cast<const char *>(start));
    const unsigned char *end = sqlite3_column_text(stmt, 5);
    if (end)
        bp.endDate = Date::fromString(reinterpret_cast<const char *>(end));
    return bp;
}
int BudgetPeriodRepository::bindBudgetPeriod(sqlite3_stmt *stmt, const BudgetPeriod &period)
{
    return 0;
}
BudgetPeriodRepository::BudgetPeriodRepository(sqlite3 *db) : BaseRepository(db)
{
    LOG_DEBUG("\n BudgetPeriodRepository created");
}

bool BudgetPeriodRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/budgetperiods.sql");
}
int BudgetPeriodRepository::insertBudgetPeriod(const BudgetPeriod &period)
{
    const char *sql = "insert into budget_periods (label, year, half, start_date, end_date) values (?,?,?,?,?)";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }

    sqlite3_bind_text(stmt, 1, period.label.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 2, period.year);
    sqlite3_bind_int(stmt, 3, period.half);
    sqlite3_bind_text(stmt, 4, period.startDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, period.endDate.to_string().c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
}
bool BudgetPeriodRepository::updateBudgetPeriod(const BudgetPeriod &period)
{
    const char *sql = "update budget_periods set label = ?,  year = ?, half = ?, start_date = ?, end_date = ? where id = ?";
    return execute(sql, [&period](sqlite3_stmt *stmt)
                   {
                       sqlite3_bind_text(stmt, 1, period.label.c_str(), -1, SQLITE_TRANSIENT);
                       sqlite3_bind_int(stmt, 2, period.year);
                       sqlite3_bind_int(stmt, 3, period.half);
                       sqlite3_bind_text(stmt, 4, period.startDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
                       sqlite3_bind_text(stmt, 5, period.endDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
                       sqlite3_bind_int(stmt, 6, period.id); });
}

bool BudgetPeriodRepository::deleteBudgetPeriod(int id)
{
    const char *sql = "DELETE FROM budget_periods where id = ?";
    return execute(sql, [&id](sqlite3_stmt *stmt)
                   { sqlite3_bind_int(stmt, 1, id); });
}
std::optional<BudgetPeriod> BudgetPeriodRepository::getById(int id) const
{
    const char *sql = "select * from budget_periods where id = ?";
    return querySingle<BudgetPeriod>(sql, mapBudgetPeriod, [id](sqlite3_stmt *stmt)
                                     { sqlite3_bind_int(stmt, 1, id); });
}
std::vector<BudgetPeriod> BudgetPeriodRepository::getAll() const
{
    const char *sql = "select * from budget_periods";
    return query<BudgetPeriod>(sql, mapBudgetPeriod);
}
