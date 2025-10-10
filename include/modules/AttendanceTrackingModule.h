#pragma once
#include <iostream>
#include "../ActionDispatcher.h"

class AttendanceTrackingModule {
public:
    AttendanceTrackingModule();
    AttendanceTrackingModule(ActionDispatcher& dispatcher);
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
