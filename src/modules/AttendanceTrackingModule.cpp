#include "../../Include/modules/AttendanceTrackingModule.h"
#include <iostream>

AttendanceTrackingModule::AttendanceTrackingModule() 
{
}
AttendanceTrackingModule::AttendanceTrackingModule(ActionDispatcher& dispatcher) 
{;
    dispatcher.regAction("attendance-tracking", "upload_time_logs", std::bind(&AttendanceTrackingModule::uploadTimeLogsFromCSV, this));
    dispatcher.regAction("attendance-tracking", "upload_time_logs", std::bind(&AttendanceTrackingModule::uploadTimeLogsFromAPI, this));
    dispatcher.regAction("attendance-tracking", "view_raw_logs",  std::bind(&AttendanceTrackingModule::viewRawLogs, this));
    dispatcher.regAction("attendance-tracking", "compute_overtime", std::bind(&AttendanceTrackingModule::computeOvertime, this));
    dispatcher.regAction("attendance-tracking", "compute_tardiness",  std::bind(&AttendanceTrackingModule::computeTardiness, this));
    dispatcher.regAction("attendance-tracking", "compute_absences", std::bind(&AttendanceTrackingModule::computeAbsences, this));
    dispatcher.regAction("attendance-tracking", "compute_undertime",  std::bind(&AttendanceTrackingModule::computeUndertime, this));
    dispatcher.regAction("attendance-tracking", "manual_edit_attendance", std::bind(&AttendanceTrackingModule::editAttendanceManually, this));
}
AttendanceTrackingModule::~AttendanceTrackingModule() 
{
    std::cout << "AttendanceTrackingModule destroyed\n";
}
void AttendanceTrackingModule::uploadTimeLogsFromCSV()
{std::cout << "AttendanceTrackingModule::uploadTimeLogsFromCSV()" << std::endl;
};
void AttendanceTrackingModule::uploadTimeLogsFromAPI()
{
    std::cout << "AttendanceTrackingModule::uploadTimeLogsFromAPI()" << std::endl;
};
void AttendanceTrackingModule::viewRawLogs()
{
    std::cout << "AttendanceTrackingModule::viewRawLogs()" << std::endl;
};
void AttendanceTrackingModule::computeOvertime()
{
    std::cout << "AttendanceTrackingModule::computeOvertime()" << std::endl;
};
void AttendanceTrackingModule::computeTardiness()
{
    std::cout << "AttendanceTrackingModule::computeTardiness()" << std::endl;
};
void AttendanceTrackingModule::computeAbsences()
{
    std::cout << "AttendanceTrackingModule::computeAbsences()" << std::endl;
};
void AttendanceTrackingModule::computeUndertime()
{
    std::cout << "AttendanceTrackingModule::computeUndertime()" << std::endl;
};
void AttendanceTrackingModule::editAttendanceManually()
{
    std::cout << "AttendanceTrackingModule::editAttendanceManually()" << std::endl;
};
