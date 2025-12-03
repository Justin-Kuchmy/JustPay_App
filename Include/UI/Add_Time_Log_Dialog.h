#ifndef  ADDTIMELOGDIALOG_H
#define  ADDTIMELOGDIALOG_H

#include <QDialog> 
#include <QWidget> 
#include "Models/DataObjects.h"

namespace Ui
{
    class AddTimeLogDialog;
}

class AddTimeLogDialog: public QDialog
{
    Q_OBJECT;
    public:
        AddTimeLogDialog(QWidget *parent = nullptr);
        ~AddTimeLogDialog();
        AttendanceLog getTimeLogData();
        void setEmployeeList(std::vector<Employee> &emps);
        void changeMode(std::string mode);
        void getLogData(int employeeId);

private slots:
        void onOkayClicked();
        void onCancelClicked();
        void onSelectTypeClicked();
        void onEmployeeSelected();
private:
    Ui::AddTimeLogDialog* ui; 
    std::string jsonDataFromDialog;  
    AttendanceLog al;   
    std::vector<Employee> allEmployees; 
    AttendanceLog selectedAttendanceLog;

};
#endif