#ifndef EMPLOYEEDETAILSWIDGET_H
#define EMPLOYEEDETAILSWIDGET_H

#include <QDialog>
#include "../Include/Models/DataObjects.h"
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include "Services/EmployeeService.h"
#include "UI/Edit_Employee_Details.h"
#include "UI/Edit_Dependent_Details.h"
#include "UI/Edit_Emergency_Contact_Details.h"
#include "UI/Loan_Ledger_Widget.h"
#include "UI/Employee_Attendance_Widget.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"

namespace Ui {
class EmployeeDetailsWidget;
}

class EmployeeDetailsWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit EmployeeDetailsWidget(QWidget *parent = nullptr);
    ~EmployeeDetailsWidget();   

private slots:
    void onEmployeeSelected(QString& empID);
    QString getSelectedEmployee() const;
    void onEmployeeSearchBoxChanged(const QString& text);
private:
    Ui::EmployeeDetailsWidget *ui;
    Employee m_Employee;
    std::vector<Employee>* employees;
    EditEmployeeDetails* editEmployeeForm;
    EditEmergencyContactDetails* editContactForm;
    EditDependentDetails* editDependentForm;
    LoanLedgerWidget* loanLedgetWidget;
    EmployeeAttendanceWidget* employeeAttendanceLogWidget;
    void refreshEmployeeList(const QString &filter = QString());
    bool loadEmployeeDetails(QString& empID);
    std::optional<Contact> optContact;
    std::optional<Dependent> optDependent;
    std::vector<LoanLedger> optLoanLedgers;
    std::vector<AttendanceLog> optAttendanceLogs;
    
};


#endif