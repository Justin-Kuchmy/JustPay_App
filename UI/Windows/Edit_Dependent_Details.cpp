#include "include/Generated/ui_edit_dependent_details.h"
#include "UI/Edit_Dependent_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"

EditDependentDetails::EditDependentDetails(Employee &emp, QWidget *parent): QWidget(parent), ui(new Ui::EditDependentDetails), a_Employee(emp)
{
    ui->setupUi(this); 
};

EditDependentDetails::~EditDependentDetails()
{
    delete ui;
};

void EditDependentDetails::setEmployeeContext()
{
ui->nameLineEdit->setText(QString::fromStdString(a_Employee.dependent.name));
ui->relationLineEdit->setText(QString::fromStdString(a_Employee.dependent.relation));
ui->bdayDateEdit->setDate(QDate::fromString(QString::fromStdString(to_string(a_Employee.dependent.birthday))));
}