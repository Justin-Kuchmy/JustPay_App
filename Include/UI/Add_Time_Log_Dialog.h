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
        AddTimeLogDialog(const AddTimeLogDialog&) = delete; 
        AddTimeLogDialog& operator=(const AddTimeLogDialog&) = delete;
        AttendanceLog getTimeLogData();
        void setEmployeeList(std::vector<Employee> &emps);
        void changeMode(std::string mode);
        void getLogData(int employeeId);
        void getFormData();

private slots:
        void onOkayClicked();
        void onSaveClicked();
        void onCancelClicked();
        void onSelectTypeClicked();
        void onEmployeeSelected();
private:
    Ui::AddTimeLogDialog* ui; 
    std::string jsonDataFromDialog;  
    std::vector<Employee> allEmployees; 
    AttendanceLog selectedAttendanceLog;
    Employee selectedEmployee;

};
#endif