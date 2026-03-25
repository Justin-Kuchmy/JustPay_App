#include "Repositories/BudgetPeriodRepository.h"

BudgetPeriod BudgetPeriodRepository::mapBudgetPeriod(sqlite3_stmt *stmt)
{
    return {};
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
    return 0;
}
bool BudgetPeriodRepository::updateBudgetPeriod(const BudgetPeriod &period)
{
    return false;
}
bool BudgetPeriodRepository::deleteBudgetPeriod(int id)
{
    return false;
}
std::optional<BudgetPeriod> BudgetPeriodRepository::getById(int id) const
{
    return std::nullopt;
}
std::vector<BudgetPeriod> BudgetPeriodRepository::getAll() const
{
    return {};
}
