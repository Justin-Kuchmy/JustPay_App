#include "include/Generated/ui_edit_dependent_details.h"
#include "UI/Edit_Dependent_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"

EditDependentDetails::EditDependentDetails(QWidget *parent): QWidget(parent), ui(new Ui::EditDependentDetails)
{
    ui->setupUi(this); 
};

EditDependentDetails::~EditDependentDetails()
{
    delete ui;
};

void EditDependentDetails::setEmployeeContext()
{

}