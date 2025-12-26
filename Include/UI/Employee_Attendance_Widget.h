#ifndef EMPLOYEEATTENDANCEWIDGET_H
#define EMPLOYEEATTENDANCEWIDGET_H
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "Models/DataObjects.h"
#include "Models/QObjects.h"

namespace Ui {
    class EmployeeAttendanceWidget;
}
class EmployeeAttendanceWidget : public QWidget {
    Q_OBJECT
public:
    EmployeeAttendanceWidget(QWidget *parent = nullptr);
    ~EmployeeAttendanceWidget();
    EmployeeAttendanceWidget(const EmployeeAttendanceWidget&) = delete; 
    EmployeeAttendanceWidget& operator=(const EmployeeAttendanceWidget&) = delete;
    void setEmployeeAttendanceContext(std::vector<AttendanceLog> *attendanceLog, std::string& employeeId);
private slots:

private:
    Ui::EmployeeAttendanceWidget *ui;
    std::vector<AttendanceLog>* attendanceLogs = nullptr;
    AttendanceLogModel* model;
    QSortFilterProxyModel* proxyModel;

    // LoanLedger* selectedLoanLedger = nullptr;
    // LoanLedger m_LoanLedger;
    std::string employeeId;
};
#endif