
#include "Repositories/AttendanceLogRepository.h"

AttendanceLogRepository::AttendanceLogRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\n AttendanceLogRepository created" << std::endl;
};

bool AttendanceLogRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/attendancelog.sql");
};

void AttendanceLogRepository::bindAttendanceLog(sqlite3_stmt *stmt, const AttendanceLog &al)
{
    int column{1};
    sqlite3_bind_text(stmt, column++, al.employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, al.logDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, column++, al.lateByMinute);
    sqlite3_bind_int(stmt, column++, al.underTimeByMinute);
    sqlite3_bind_int(stmt, column++, al.overTimeByMinute);
    sqlite3_bind_int(stmt, column++, al.isAbsent);
    sqlite3_bind_text(stmt, column++, al.notes.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, al.overtimeJson.c_str(), -1, SQLITE_TRANSIENT);
}
AttendanceLog AttendanceLogRepository::mapAttendanceLog(sqlite3_stmt *stmt)
{
    AttendanceLog al;
    al.logId = sqlite3_column_int(stmt, 0);
    al.employeeId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    const unsigned char *text = sqlite3_column_text(stmt, 2);
    if (text)
        al.logDate = Date::fromString(reinterpret_cast<const char *>(text));
    else
        al.logDate = Date(1900, 1, 1);
    al.lateByMinute = sqlite3_column_int(stmt, 3);
    al.underTimeByMinute = sqlite3_column_int(stmt, 4);
    al.overTimeByMinute = sqlite3_column_int(stmt, 5);
    al.isAbsent = sqlite3_column_int(stmt, 6);

    const unsigned char *notes = sqlite3_column_text(stmt, 7);
    al.notes = notes ? reinterpret_cast<const char *>(notes) : "";

    const unsigned char *overtime = sqlite3_column_text(stmt, 8);
    al.overtimeJson = overtime ? reinterpret_cast<const char *>(overtime) : "";

    al.overtimeObj = Overtime::fromJson(al.overtimeJson);

    return al;
};

// create
sqlite3_int64 AttendanceLogRepository::insertAttendanceLog(const AttendanceLog &al)
{
    const char *sql = "INSERT INTO attendance_log (employeeId, log_date, late_min, undertime_min, overtime_min, absent, notes, overtime_json) VALUES (?,?,?,?,?,?,?,?)";
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        return 0;
    }
    AttendanceLogRepository::bindAttendanceLog(stmt, al);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_DEBUG("SQL insert failed: " << sqlite3_errmsg(db));
        return 0;
    }
    return sqlite3_last_insert_rowid(db);
};

// read
std::optional<AttendanceLog> AttendanceLogRepository::getById(int logId)
{
    const char *sql = "SELECT * from attendance_log where logId = ?";
    sqlite3_stmt *stmt = nullptr;
    std::optional<AttendanceLog> result = std::nullopt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "Failed to prepare: " << sqlite3_errmsg(db) << std::endl;
        return result;
    }
    else
        sqlite3_bind_int(stmt, 1, logId);
    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        result = mapAttendanceLog(stmt);
    }
    sqlite3_finalize(stmt);
    return result;
};

std::vector<AttendanceLog> AttendanceLogRepository::getAllById(std::string id)
{
    const char *sql = "SELECT * from attendance_log where employeeId = ?";

    return query<AttendanceLog>(sql, mapAttendanceLog, [id](sqlite3_stmt *stmt)
                                { sqlite3_bind_text(stmt, 1, id.c_str(), -1, SQLITE_TRANSIENT); });
}

std::vector<AttendanceLog> AttendanceLogRepository::getAll()
{
    const char *sql = "SELECT * from attendance_log";
    auto results = this->query<AttendanceLog>(sql, mapAttendanceLog);

    if (results.size() > 0)
    {
        return results;
    }
    else
    {
        LOG_DEBUG("failed to get anything from the db");
        return {};
    }
}

// update
bool AttendanceLogRepository::updateAttendanceLog(const AttendanceLog &al)
{
    const char *sql = R"(update attendance_log set 
    employeeId= ?,
    log_date=?,
    late_min=?,
    undertime_min=?,
    overtime_min=?,
    absent=?,
    notes=?,
    overtime_json=? 
    where logId = ?;
    )";
    auto res = AttendanceLogRepository::execute(sql, [&al](sqlite3_stmt *stmt)
                                                { bindAttendanceLog(stmt, al);
                                                sqlite3_bind_int(stmt, 9, al.logId); });
    return res;
};

// delete
bool AttendanceLogRepository::deleteAttendanceLog(int logId)
{
    std::string sql = "DELETE FROM attendance_log WHERE logId = ?";
    return AttendanceLogRepository::execute(sql, [&logId](sqlite3_stmt *stmt)
                                            { sqlite3_bind_int(stmt, 1, logId); });
}

std::string AttendanceLogRepository::getLastAttendanceLogId()
{
    LOG_DEBUG("AttendanceLogRepository::getLastAttendanceLogId not implemented");
    return "";
};
