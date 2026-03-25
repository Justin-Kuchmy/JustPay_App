
#include "Repositories/LeaveRepository.h"

LeaveRepository::LeaveRepository(sqlite3 *db) : BaseRepository(db)
{
    LOG_DEBUG("\n LeaveRepository created");
};
bool LeaveRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/employeeleavebalances.sql");
};

EmployeeLeaveBalance LeaveRepository::mapEmployeeLeaveBalance(sqlite3_stmt *stmt)
{
    EmployeeLeaveBalance eL{};
    eL.employeeId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    eL.year = sqlite3_column_int(stmt, 1);
    eL.totalLeaveEarned = sqlite3_column_double(stmt, 2);
    eL.leaveUsed = sqlite3_column_double(stmt, 3);
    eL.unusedLeaveDays = (eL.totalLeaveEarned - eL.leaveUsed);
    return eL;
}

std::optional<EmployeeLeaveBalance> LeaveRepository::getByEmployeeAndYear(const std::string &employeeId, int year)
{
    const char *sql = "select * from employee_leave_balances where employee_id = ? and year = ?";
    sqlite3_stmt *stmt = nullptr;
    std::optional<EmployeeLeaveBalance> result = std::nullopt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return result;
    }
    else
    {
        sqlite3_bind_text(stmt, 1, employeeId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, year);
    }
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
        result = mapEmployeeLeaveBalance(stmt);
    return result;
}