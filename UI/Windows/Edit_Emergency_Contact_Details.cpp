#include "include/Generated/ui_edit_emergency_contact_details.h"
#include "UI/Edit_Emergency_Contact_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"

EditEmergencyContactDetails::EditEmergencyContactDetails(QWidget *parent): QWidget(parent), ui(new Ui::EditEmergencyContactDetails)
{
    ui->setupUi(this); 
};

EditEmergencyContactDetails::~EditEmergencyContactDetails()
{

};

void EditEmergencyContactDetails::setEmployeeContext()
{

}