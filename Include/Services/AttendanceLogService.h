#ifndef ATTENDANCELOGSERVICE_H
#define ATTENDANCELOGSERVICE_H
#include "Repositories/AttendanceLogRepository.h"

class AttendanceLogService
{
    protected:
        AttendanceLogRepository& repo;
    public:
        explicit AttendanceLogService(AttendanceLogRepository& r);

        //CREATE
        int addAttendanceLog(const AttendanceLog& AttendanceLog);

        //READ
        std::optional<AttendanceLog> getAttendanceLogByID(int id);

        std::vector<AttendanceLog> getAllAttendanceLogsById(std::string id);
        std::vector<AttendanceLog> getAllAttendanceLogs();

        //UPDATE
        bool updateAttendanceLog(const AttendanceLog& e);
        
        //DELETE
        bool deleteAttendanceLog(int id);
 
};

#endif