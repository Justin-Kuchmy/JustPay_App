#include "include/Generated/ui_edit_emergency_contact_details.h"
#include "UI/Edit_Emergency_Contact_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"

EditEmergencyContactDetails::EditEmergencyContactDetails(Employee &emp,QWidget *parent): QWidget(parent), ui(new Ui::EditEmergencyContactDetails), a_Employee(emp)
{
    ui->setupUi(this); 
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

}