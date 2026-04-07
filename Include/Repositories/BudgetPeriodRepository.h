#ifndef BUDGETPERIODREPOSITORY_H
#define BUDGETPERIODREPOSITORY_H

#include "Repositories/BaseRepository.h"
#include "Models/Primitives/date.h"
#include <sqlite3.h>

class BudgetPeriodRepository : public BaseRepository
{
private:
    static BudgetPeriod mapBudgetPeriod(sqlite3_stmt *stmt);
    static int bindBudgetPeriod(sqlite3_stmt *stmt, const BudgetPeriod &period);

public:
    explicit BudgetPeriodRepository(sqlite3 *db);

    bool createTable() const override;

    int insertBudgetPeriod(const BudgetPeriod &period);
    bool updateBudgetPeriod(const BudgetPeriod &period);
    bool deleteBudgetPeriod(int id);

    std::optional<BudgetPeriod> getById(int id) const;
    std::vector<BudgetPeriod> getAll() const;
};

#endif