
#include "Repositories/AttendanceLogRepository.h"




AttendanceLogRepository::AttendanceLogRepository(sqlite3* db): BaseRepository(db)
{
    std::cout << "\n AttendanceLogRepository created" << std::endl;
};

std::string AttendanceLogRepository::getCreateTableSQL() const
{
        return R"(
        PRAGMA foreign_keys = OFF;
        DROP TABLE IF EXISTS attendance_log;

        CREATE TABLE IF NOT EXISTS attendance_log (
            logId INTEGER PRIMARY KEY AUTOINCREMENT,
            employeeId TEXT,
            log_date TEXT,
            late_min INTEGER,
            undertime_min INTEGER,
            overtime_min INTEGER,
            absent INTEGER default 0,
            notes TEXT,
            overtime_json TEXT,
            FOREIGN KEY (employeeId) REFERENCES employees(employeeId) ON DELETE SET NULL ON UPDATE CASCADE
        );

        -- LOGS
        INSERT INTO attendance_log (employeeId, log_date, late_min, undertime_min, overtime_min, absent, notes, overtime_json) VALUES
        ('00-0001','2025-11-20',15,0,0,0,"Late by 15 minutes","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
        ('01-0002','2025-11-20',0,5,0,0,"Left early by 5 minutes","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
        ('01-0003','2025-11-21',0,0,120,0,"Overtime: 2 hours","{'regular':120,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':30}"),
        ('01-0003','2025-11-22',0,0,30,0,"Overtime: 30 minutes","{'regular':30,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':30}"),
        ('01-0003','2025-11-23',0,0,60,0,"Overtime: 1 hour","{'regular':60,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':30}"),
        ('02-0004','2025-11-21',10,0,0,0,"Late by 10 minutes","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
        ('02-0005','2025-11-22',5,0,15,0,"Late by 5 minutes; Overtime: 15 minutes","{'regular':15,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':15}"),
        ('03-0006','2025-11-22',0,0,0,1,"Absent","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
        ('03-0007','2025-11-23',8,0,0,0,"Late by 8 minutes","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
        ('04-0008','2025-11-23',12,0,20,0,"Late by 12 minutes; Overtime: 20 minutes","{'regular':20,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':20}"),
        ('04-0009','2025-11-24',0,0,0,0,"On time","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
        ('05-0010','2025-11-24',20,0,0,0,"Late by 20 minutes","{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}");
        )";

};

AttendanceLog AttendanceLogRepository::mapAttendanceLog(sqlite3_stmt* stmt)
{
    AttendanceLog al;
    al.logId = sqlite3_column_int(stmt, 0);
    al.employeeId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    const unsigned char* text = sqlite3_column_text(stmt, 2);
    if (text) al.logDate = Date::fromString(reinterpret_cast<const char*>(text));
    else al.logDate = Date(1900,1,1);
    al.lateByMinute  = sqlite3_column_int(stmt, 3);
    al.underTimeByMinute = sqlite3_column_int(stmt, 4);
    al.overTimeByMinute = sqlite3_column_int(stmt, 5);
    al.isAbsent = sqlite3_column_int(stmt, 6);

    const unsigned char* notes = sqlite3_column_text(stmt, 7);
    al.notes = notes ? reinterpret_cast<const char*>(notes) : "";

    const unsigned char* overtime = sqlite3_column_text(stmt, 8);
    al.overtimeJson = overtime ? reinterpret_cast<const char*>(overtime) : "";
    
    return al;
};

//create
sqlite3_int64 AttendanceLogRepository::insertAttendanceLog(const AttendanceLog& al)
{
    const char* sql = "INSERT INTO attendance_log (employeeId, log_date, late_min, undertime_min, overtime_min, absent, notes, overtime_json) VALUES (?,?,?,?,?,?,?,?)";
    sqlite3_stmt* stmt = nullptr;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
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
        return 0;  // Insert failed
    return static_cast<sqlite3_int64>(sqlite3_last_insert_rowid(db));
};

//read
std::optional<AttendanceLog> AttendanceLogRepository::getById(int logId)
{
    const char* sql = "SELECT * from attendance_log where logId == ?";
    sqlite3_stmt* stmt = nullptr;
    std::optional<AttendanceLog> result = std::nullopt;
    if(sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
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
    
    auto results = this->query<AttendanceLog>(sql,mapAttendanceLog);
    

    if(results.size() > 0)
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
    auto results = this->query<AttendanceLog>(sql,mapAttendanceLog);

    if(results.size() > 0)
    {
        return results;
    }
    else
    {
        LOG_DEBUG("failed to get anything from the db");
        return {};
    }
}

//update
bool AttendanceLogRepository::updateAttendanceLog(const AttendanceLog& al)
{
    LOG_DEBUG("logId " << al.logId);
    LOG_DEBUG("updateAttendanceLog " << al.lateByMinute);
    std::string sql = std::format("update attendance_log set employeeId='{}', log_date='{}', late_min={}, undertime_min={}, overtime_min={}, absent={}, notes='{}', overtime_json=\"{}\" where logId = '{}'",
        al.employeeId,
        al.logDate.to_string(),
        al.lateByMinute ,
        al.underTimeByMinute,
        al.overTimeByMinute,
        al.isAbsent,
        al.notes,
        al.overtimeJson,
        al.logId
    );
    return AttendanceLogRepository::execute(sql);
};

//delete
bool AttendanceLogRepository::deleteAttendanceLog(int logId)
{
    std::string sql = std::format("DELETE from attendance_log where logId = '{}'",logId);
    return AttendanceLogRepository::execute(sql);
};

std::string AttendanceLogRepository::getLastAttendanceLogId()
{
LOG_DEBUG("AttendanceLogRepository::getLastAttendanceLogId not implemented");
    return "";
};

