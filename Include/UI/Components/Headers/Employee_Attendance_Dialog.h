#ifndef EMPLOYEEATTENDANCEDIALOG_H
#define EMPLOYEEATTENDANCEDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class EmployeeAttendanceDialog;
}

class EmployeeAttendanceDialog : public QDialog {
    Q_OBJECT

public:
    explicit EmployeeAttendanceDialog(QWidget *parent = nullptr);
    ~EmployeeAttendanceDialog();
    
private slots:


private:
    Ui::EmployeeAttendanceDialog *ui;
};


#endif