
#include "Repositories/AttendanceLogRepository.h"

AttendanceLogRepository::AttendanceLogRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\n AttendanceLogRepository created" << std::endl;
};

std::string AttendanceLogRepository::getCreateTableSQL() const
{
    QFile file(":/resources/sql/attendancelog.sql");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_DEBUG("Failed to open file");
        return "";
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return content.toStdString();
};

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
    sqlite3_bind_text(stmt, 1, al.employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, al.logDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, al.lateByMinute);
    sqlite3_bind_int(stmt, 4, al.underTimeByMinute);
    sqlite3_bind_int(stmt, 5, al.overTimeByMinute);
    sqlite3_bind_int(stmt, 6, al.isAbsent);
    sqlite3_bind_text(stmt, 7, al.notes.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8, al.overtimeJson.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
        return 0; // Insert failed
    return static_cast<sqlite3_int64>(sqlite3_last_insert_rowid(db));
};

// read
std::optional<AttendanceLog> AttendanceLogRepository::getById(int logId)
{
    const char *sql = "SELECT * from attendance_log where logId == ?";
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
    std::string sql = std::format("SELECT * from attendance_log where employeeId = '{}'", id);

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

std::vector<AttendanceLog> AttendanceLogRepository::getAll()
{
    std::string sql = std::format("SELECT * from attendance_log");
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
    LOG_DEBUG("logId " << al.logId);
    LOG_DEBUG("updateAttendanceLog " << al.lateByMinute);
    std::string sql = std::format("update attendance_log set employeeId='{}', log_date='{}', late_min={}, undertime_min={}, overtime_min={}, absent={}, notes='{}', overtime_json=\"{}\" where logId = '{}'",
                                  al.employeeId,
                                  al.logDate.to_string(),
                                  al.lateByMinute,
                                  al.underTimeByMinute,
                                  al.overTimeByMinute,
                                  al.isAbsent,
                                  al.notes,
                                  al.overtimeJson,
                                  al.logId);
    return AttendanceLogRepository::execute(sql);
};

// delete
bool AttendanceLogRepository::deleteAttendanceLog(int logId)
{
    std::string sql = std::format("DELETE from attendance_log where logId = '{}'", logId);
    return AttendanceLogRepository::execute(sql);
};

std::string AttendanceLogRepository::getLastAttendanceLogId()
{
    LOG_DEBUG("AttendanceLogRepository::getLastAttendanceLogId not implemented");
    return "";
};
