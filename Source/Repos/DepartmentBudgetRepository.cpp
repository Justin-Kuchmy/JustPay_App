#include "Repositories/DepartmentBudgetRepository.h"

DepartmentBudget DepartmentBudgetRepository::mapDepartmentBudget(sqlite3_stmt *stmt)
{
    DepartmentBudget dBudget{};
    dBudget.id = sqlite3_column_int(stmt, 0);
    dBudget.department = static_cast<Department>(sqlite3_column_int(stmt, 1));
    dBudget.period_Id = sqlite3_column_int(stmt, 2);
    dBudget.allocatedAmount = sqlite3_column_double(stmt, 3);
    dBudget.notes = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    return dBudget;
}
void DepartmentBudgetRepository::bindDepartmentBudget(sqlite3_stmt *stmt, const DepartmentBudget &budget)
{
    sqlite3_bind_int(stmt, 1, static_cast<int>(budget.department));
    sqlite3_bind_int(stmt, 2, budget.period_Id);
    sqlite3_bind_double(stmt, 3, budget.allocatedAmount);
    sqlite3_bind_text(stmt, 4, budget.notes.c_str(), -1, SQLITE_TRANSIENT);
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
    const char *sql = "insert into department_budgets (department, period_id, allocated_amount, notes) values (?,?,?,?)";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }

    bindDepartmentBudget(stmt, budget);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
}
bool DepartmentBudgetRepository::updateDepartmentBudget(const DepartmentBudget &budget)
{
    const char *sql = "update department_budgets set department = ?,  period_id = ?, allocated_amount = ?, notes = ? where id = ?";
    return execute(sql, [&budget](sqlite3_stmt *stmt)
                   { bindDepartmentBudget(stmt, budget);
                    sqlite3_bind_int(stmt, 5, budget.id); });
}
bool DepartmentBudgetRepository::deleteDepartmentBudget(int id)
{
    const char *sql = "DELETE FROM department_budgets where id = ?";
    return execute(sql, [&id](sqlite3_stmt *stmt)
                   { sqlite3_bind_int(stmt, 1, id); });
}
std::optional<DepartmentBudget> DepartmentBudgetRepository::getById(int id) const
{
    const char *sql = "select * from department_budgets where id = ?";
    return querySingle<DepartmentBudget>(sql, mapDepartmentBudget, [id](sqlite3_stmt *stmt)
                                         { sqlite3_bind_int(stmt, 1, id); });
}
std::vector<DepartmentBudget> DepartmentBudgetRepository::getByPeriod(int period_id) const
{
    const char *sql = "select * from department_budgets where period_id = ?";
    return query<DepartmentBudget>(sql, mapDepartmentBudget, [period_id](sqlite3_stmt *stmt)
                                   { sqlite3_bind_int(stmt, 1, period_id); });
}
std::vector<DepartmentBudget> DepartmentBudgetRepository::getAll() const
{
    const char *sql = "select * from department_budgets";
    return query<DepartmentBudget>(sql, mapDepartmentBudget);
}
