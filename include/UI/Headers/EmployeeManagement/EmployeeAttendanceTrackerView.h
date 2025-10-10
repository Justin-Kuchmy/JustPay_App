#ifndef EMPLOYEETRACKER_H
#define EMPLOYEETRACKER_H


#include <QApplication>
#include <QScreen>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "../Baseview.h"

class EmployeeAttendanceTrackerView : public Baseview {
    Q_OBJECT

public:
    explicit EmployeeAttendanceTrackerView(QWidget *parent = nullptr);

signals:
    void backToMainRequested();
};

#endif