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
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#include "Utils/Log.h"

namespace Ui {
class EmployeeDetailsWidget;
}

class EmployeeDetailsWidget : public QWidget {
    Q_OBJECT

public:
    explicit EmployeeDetailsWidget(QWidget *parent = nullptr);
    ~EmployeeDetailsWidget();   
private slots:
    void onOKClicked();
    void onCancelClicked();
    void openEditContactDialog();
    void openEditDependentDialog();
    void onEmployeeSelected(QString& empID);
    QString getSelectedEmployee() const;
    void onEmployeeSearchBoxChanged(const QString& text);
private:
    Ui::EmployeeDetailsWidget *ui;
    Employee a_Employee;
    std::vector<Employee>* employees;
    EditEmployeeDetails* editEmployeeForm;
    EditEmergencyContactDetails* editContactForm;
    EditDependentDetails* editDependentForm;
    void refreshEmployeeList(const QString &filter = QString());
    void loadEmployeeDetails();
};


#endif