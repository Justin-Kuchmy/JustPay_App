#pragma once
#include <iostream>

class AttendanceTrackingModule {
public:
    AttendanceTrackingModule();
    ~AttendanceTrackingModule();
    void uploadTimeLogsFromCSV();
    void uploadTimeLogsFromAPI();
    void viewRawLogs();
    void computeOvertime();
    void computeTardiness();
    void computeAbsences();
    void computeUndertime();
    void editAttendanceManually();
};
