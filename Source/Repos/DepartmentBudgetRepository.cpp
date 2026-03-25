#include "Repositories/DepartmentBudgetRepository.h"

DepartmentBudget DepartmentBudgetRepository::mapDepartmentBudget(sqlite3_stmt *stmt)
{
    return {};
}
int DepartmentBudgetRepository::bindDepartmentBudget(sqlite3_stmt *stmt, const DepartmentBudget &budget)
{
    return 0;
}
DepartmentBudgetRepository::DepartmentBudgetRepository(sqlite3 *db) : BaseRepository(db)
{
    LOG_DEBUG("\n DepartmentBudgetRepository created");
}

bool DepartmentBudgetRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/departmentbudgets.sql");
}
int DepartmentBudgetRepository::insertDepartmentBudget(const DepartmentBudget &budget)
{
    return 0;
}
bool DepartmentBudgetRepository::updateDepartmentBudget(const DepartmentBudget &budget)
{
    return false;
}
bool DepartmentBudgetRepository::deleteDepartmentBudget(int id)
{
    return false;
}
std::optional<DepartmentBudget> DepartmentBudgetRepository::getById(int id) const
{
    return std::nullopt;
}
std::vector<DepartmentBudget> DepartmentBudgetRepository::getByPeriod(int periodId) const
{
    return {};
}
std::vector<DepartmentBudget> DepartmentBudgetRepository::getAll() const
{
    return {};
}
