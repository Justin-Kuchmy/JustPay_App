#include "include/Generated/ui_add_employee_dialog.h"
#include "UI/Add_Employee_Dialog.h"
#include <QWidget>
#include <QListWidgetItem>
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Utils/DialogFactory.h"
#include "UI/Add_Dependent_Dialog.h"
#include "UI/Add_Emergency_Contact_Dialog.h"
#include "Services/AppContext.h"

AddEmployeeDialog::AddEmployeeDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddEmployeeDialog)
{
    ui->setupUi(this); 
    
    ui->departmentComboBox->addItems({
        QString::fromStdString(department_to_string(Department::HR)),
        QString::fromStdString(department_to_string(Department::Finance)),
        QString::fromStdString(department_to_string(Department::IT)),
        QString::fromStdString(department_to_string(Department::Operations)),
        QString::fromStdString(department_to_string(Department::Sales)),
        QString::fromStdString(department_to_string(Department::Marketing)),
        QString::fromStdString(department_to_string(Department::Admin)),
        QString::fromStdString(department_to_string(Department::Legal)),
        QString::fromStdString(department_to_string(Department::Engineering))
    }); //Department enum
    ui->jobLevelComboBox->addItems({
        QString::fromStdString(JobLevel_to_string(JobLevel::RankAndFile)),
        QString::fromStdString(JobLevel_to_string(JobLevel::Supervisory)),
        QString::fromStdString(JobLevel_to_string(JobLevel::Managerial)),
        QString::fromStdString(JobLevel_to_string(JobLevel::Executive)),
        QString::fromStdString(JobLevel_to_string(JobLevel::BoardOwnership))
    });   //JobLevel enum
    ui->empStatusComboBox->addItems({
        QString::fromStdString(Status_to_string(EmploymentStatus::Regular)),
        QString::fromStdString(Status_to_string(EmploymentStatus::Probationary)),
        QString::fromStdString(Status_to_string(EmploymentStatus::Contractual)),
        QString::fromStdString(Status_to_string(EmploymentStatus::ProjectBased)),
        QString::fromStdString(Status_to_string(EmploymentStatus::Seasonal)),
        QString::fromStdString(Status_to_string(EmploymentStatus::Casual)),
        QString::fromStdString(Status_to_string(EmploymentStatus::PartTime)),
        QString::fromStdString(Status_to_string(EmploymentStatus::InternOJT)),
        QString::fromStdString(Status_to_string(EmploymentStatus::Consultant))
    });  //EmploymentStatus enum

    ui->monthlySalarySpinBox->setRange(0.00, 1'000'000'000.00);  // 1 billion max
    ui->monthlySalarySpinBox->setDecimals(2);
    //ui->monthlySalarySpinBox->setPrefix("₱ ");
    ui->monthlySalarySpinBox->setSingleStep(1000.00);

    ui->monthlyAllowancesSpinBox->setRange(0.00, 1'000'000'000.00);  // 1 billion max
    ui->monthlyAllowancesSpinBox->setDecimals(2);
    //ui->monthlyAllowancesSpinBox->setPrefix("₱ ");
    ui->monthlyAllowancesSpinBox->setSingleStep(1000.00);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddEmployeeDialog::onOKClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddEmployeeDialog::onCancelClicked);
    connect(ui->emergencyContactButton, &QPushButton::clicked, this, &AddEmployeeDialog::openAddContactDialog);
    connect(ui->dependentButton, &QPushButton::clicked, this, &AddEmployeeDialog::openAddDependentDialog);
}

AddEmployeeDialog::~AddEmployeeDialog()
{
    delete ui;
};

void AddEmployeeDialog::openAddContactDialog()
{
    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("add_contact");
    auto contactDialog = dynamic_cast<EmergencyContactDialog*>(dlg);
    if(contactDialog && contactDialog->exec() == QDialog::Accepted)   
        a_Employee.emergencyContact = contactDialog->getContactData();  
    
};

void AddEmployeeDialog::openAddDependentDialog()
{
    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("add_dependent");
    auto dependentDialog = dynamic_cast<AddDependentDialog*>(dlg);
    if(dependentDialog && dependentDialog->exec() == QDialog::Accepted)    
        a_Employee.dependent = dependentDialog->getDependentData();;  
    
};

void AddEmployeeDialog::onOKClicked()
{
    a_Employee.fullName = ui->nameLineEdit->text().toStdString();
    //a_Employee.employeeId; //this will be auto generated
    a_Employee.department = to_department(ui->departmentComboBox->currentIndex());
    a_Employee.position = ui->positionLineEdit->text().toStdString();
    a_Employee.jobLevel = to_jobLevel(ui->jobLevelComboBox->currentIndex());
    a_Employee.status = to_status(ui->empStatusComboBox->currentIndex());
    a_Employee.dateHired = from_string(ui->dateHiredDateEdit->date().toString("yyyy-MM-dd").toStdString());
    a_Employee.dateSeparation = from_string(ui->DateSeparatedDateEdit->date().toString("yyyy-MM-dd").toStdString());
    a_Employee.sssNumber = ui->sssNumberLineEdit->text().toStdString();
    a_Employee.philHealthNumber = ui->philHealthNumberLineEdit->text().toStdString();
    a_Employee.hdmfNumber = ui->hdmfNumberLineEdit->text().toStdString();
    a_Employee.tin = ui->tinLineEdit->text().toStdString();
    a_Employee.monthlyBasicSalary = ui->monthlySalarySpinBox->value();
    a_Employee.monthlyAllowances = ui->monthlyAllowancesSpinBox->value();
    a_Employee.personalEmail = ui->personEmailLineEdit->text().toStdString();
    a_Employee.isActive = ui->activeStatusCheckBox->isChecked();
    if(AppContext::instance().employeeService().addEmployee(a_Employee));
        qDebug() << "Employee " << QString::fromStdString(a_Employee.fullName) << " added!";
        accepted();
};

void AddEmployeeDialog::onCancelClicked()
{
    rejected();
};