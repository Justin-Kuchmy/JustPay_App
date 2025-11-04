#include "include/Generated/ui_edit_emergency_contact_details.h"
#include "UI/Edit_Emergency_Contact_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#define DEBUG_LOGS
#include "Utils/Log.h"


EditEmergencyContactDetails::EditEmergencyContactDetails(Employee &emp,QWidget *parent): QWidget(parent), ui(new Ui::EditEmergencyContactDetails), a_Employee(emp)
{
    ui->setupUi(this); 
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditEmergencyContactDetails::onSaveClicked);
};

EditEmergencyContactDetails::~EditEmergencyContactDetails()
{
    delete ui;
};

void EditEmergencyContactDetails::setEmployeeContext()
{
ui->nameLineEdit->setText(QString::fromStdString(a_Employee.emergencyContact.name));
ui->relationLineEdit->setText(QString::fromStdString(a_Employee.emergencyContact.relation));
ui->addressLineEdit->setText(QString::fromStdString(a_Employee.emergencyContact.address));
ui->contactNumLineEdit->setText(QString::fromStdString(a_Employee.emergencyContact.contactNo));

};


void EditEmergencyContactDetails::onSaveClicked()
{
    this->a_Employee.emergencyContact.name = ui->nameLineEdit->text().toStdString();
    this->a_Employee.emergencyContact.relation = ui->relationLineEdit->text().toStdString();
    this->a_Employee.emergencyContact.address = ui->addressLineEdit->text().toStdString();
    this->a_Employee.emergencyContact.contactNo = ui->contactNumLineEdit->text().toStdString();
    if(AppContext::instance().employeeService().updateEmployee(this->a_Employee))
    {
        LOG_DEBUG(a_Employee.emergencyContact.name << " updated!");
    }
};
