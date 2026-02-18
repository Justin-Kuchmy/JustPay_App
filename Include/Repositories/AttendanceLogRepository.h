#ifndef ATTENDANCELOGREPO_H
#define ATTENDANCELOGREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class AttendanceLogRepository : public BaseRepository
{
    static AttendanceLog mapAttendanceLog(sqlite3_stmt *stmt);

public:
    explicit AttendanceLogRepository(sqlite3 *db);

    bool createTable() const override;
    // create
    sqlite3_int64 insertAttendanceLog(const AttendanceLog &attendanceLog);
    // read
    std::optional<AttendanceLog> getById(int id);
    std::vector<AttendanceLog> getAllById(std::string id);
    std::vector<AttendanceLog> getAll();
    // update
    bool updateAttendanceLog(const AttendanceLog &emp);
    // delete
    bool deleteAttendanceLog(int id);
    std::string getLastAttendanceLogId();
};

#endif