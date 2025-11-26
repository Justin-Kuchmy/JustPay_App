
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
            overtime_json TEXT,
            FOREIGN KEY (employeeId) REFERENCES employees(employeeId) ON DELETE SET NULL ON UPDATE CASCADE
        );

        -- LOGS
        INSERT INTO attendance_log (employeeId, log_date, late_min, undertime_min, overtime_min, absent, overtime_json) VALUES
            ('00-0001','2025-11-20',15,0,0,0,"{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
            ('00-0001','2025-11-21',12,0,0,0,"{'regular':0,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
            ('00-0001','2025-11-22',7,0,45,0,"{'regular':45,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':45}"),
            ('00-0001','2025-11-23',10,0,23,0,"{'regular':23,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':20}"),
            ('00-0001','2025-11-24',25,0,30,0,"{'regular':30,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}"),
            ('00-0001','2025-11-25',5,0,60,0,"{'regular':60,'rest_day':0,'rest_day_plus':0,'legal_holiday':0,'legal_holiday_plus':0,'special_holiday':0,'special_holiday_plus':0,'rest_plus_legal':0,'rest_plus_special':0,'night_shift_diff':0}");
        )";
};

AttendanceLog AttendanceLogRepository::mapAttendanceLog(sqlite3_stmt* stmt)
{

};

//create
int AttendanceLogRepository::insertAttendanceLog(const AttendanceLog& attendanceLog)
{

};

//read
std::optional<AttendanceLog> AttendanceLogRepository::getById(int id)
{

};

//update
bool AttendanceLogRepository::updateAttendanceLog(const AttendanceLog& emp)
{

};

//delete
bool AttendanceLogRepository::deleteAttendanceLog(int id)
{

};

std::string AttendanceLogRepository::getLastDependentId()
{

};

