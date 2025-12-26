
#include "Services/AttendanceLogService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

AttendanceLogService::AttendanceLogService(AttendanceLogRepository& r): repo(r)
{
    
};

//CREATE
sqlite3_int64 AttendanceLogService::addAttendanceLog(const AttendanceLog& AttendanceLog)
{
    return this->repo.insertAttendanceLog(AttendanceLog);
};

//READ
std::optional<AttendanceLog> AttendanceLogService::getAttendanceLogByID(int id)
{
    return this->repo.getById(id);
};

std::vector<AttendanceLog> AttendanceLogService::getAllAttendanceLogs()
{
    return this->repo.getAll();
};

std::vector<AttendanceLog> AttendanceLogService::getAllAttendanceLogsById(std::string id)
{
    return this->repo.getAllById(id);
}


//UPDATE
bool AttendanceLogService::updateAttendanceLog(const AttendanceLog& e)
{
    return this->repo.updateAttendanceLog(e);
};

//DELETE
bool AttendanceLogService::deleteAttendanceLog(int id)
{
    return this->repo.deleteAttendanceLog(id);
};
