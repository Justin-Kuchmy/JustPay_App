#ifndef DEPARTMENTBUDGETREPOSITORY_H
#define DEPARTMENTBUDGETREPOSITORY_H

#include "Repositories/BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class DepartmentBudgetRepository : public BaseRepository
{
private:
    static DepartmentBudget mapDepartmentBudget(sqlite3_stmt *stmt);
    static void bindDepartmentBudget(sqlite3_stmt *stmt, const DepartmentBudget &budget);

public:
    explicit DepartmentBudgetRepository(sqlite3 *db);

    bool createTable() const override;

    int insertDepartmentBudget(const DepartmentBudget &budget);
    bool updateDepartmentBudget(const DepartmentBudget &budget);
    bool deleteDepartmentBudget(int id);

    std::optional<DepartmentBudget> getById(int id) const;
    std::vector<DepartmentBudget> getByPeriod(int periodId) const;
    std::vector<DepartmentBudget> getAll() const;
};

#endif