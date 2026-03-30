#ifndef LEAVEREPO_H
#define LEAVEREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class LeaveRepository : public BaseRepository
{
    static EmployeeLeaveBalance mapEmployeeLeaveBalance(sqlite3_stmt *stmt);

public:
    explicit LeaveRepository(sqlite3 *db);
    bool createTable() const override;
    bool insert(const EmployeeLeaveBalance &empLeaveBalance);
    std::optional<EmployeeLeaveBalance> getByEmployeeAndYear(const std::string &employeeId, int year);
};

#endif