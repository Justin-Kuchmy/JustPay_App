
#include "Services/AttendanceLogService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

AttendanceLogService::AttendanceLogService(AttendanceLogRepository& r): repo(r)
{
    
};

//CREATE
int AttendanceLogService::addAttendanceLog(const AttendanceLog& AttendanceLog)
{
    
};

//READ
std::optional<AttendanceLog> AttendanceLogService::getAttendanceLogByID(int id)
{
    
};

std::vector<AttendanceLog> AttendanceLogService::getAllAttendanceLogs()
{
    
};

//UPDATE
bool AttendanceLogService::updateAttendanceLog(const AttendanceLog& e)
{
    
};

//DELETE
bool AttendanceLogService::deleteAttendanceLog(int id)
{
    
};
