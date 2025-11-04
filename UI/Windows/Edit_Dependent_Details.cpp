#include "include/Generated/ui_edit_dependent_details.h"
#include "UI/Edit_Dependent_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#define DEBUG_LOGS
#include "Utils/Log.h"


EditDependentDetails::EditDependentDetails(Employee &emp, QWidget *parent): QWidget(parent), ui(new Ui::EditDependentDetails), a_Employee(emp)
{
    ui->setupUi(this); 
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditDependentDetails::onSaveClicked);
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
};


void EditDependentDetails::onSaveClicked()
{

    this->a_Employee.dependent.name = ui->nameLineEdit->text().toStdString();
    this->a_Employee.dependent.relation = ui->relationLineEdit->text().toStdString();
    this->a_Employee.dependent.birthday = from_string(ui->bdayDateEdit->date().toString("yyyy-MM-dd").toStdString());
    if(AppContext::instance().employeeService().updateEmployee(this->a_Employee))
    {
        LOG_DEBUG(a_Employee.dependent.name << " updated!");
    }
};
