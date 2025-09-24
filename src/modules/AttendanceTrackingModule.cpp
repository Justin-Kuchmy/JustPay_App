#include "../../include/modules/AttendanceTrackingModule.hpp"
#include <iostream>

AttendanceTrackingModule::AttendanceTrackingModule() 
{
    std::cout << "AttendanceTrackingModule created\n";
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
