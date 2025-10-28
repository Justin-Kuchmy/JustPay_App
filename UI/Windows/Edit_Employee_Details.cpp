#include "include/Generated/ui_edit_employee_details.h"
#include "UI/Edit_Employee_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"



EditEmployeeDetails::EditEmployeeDetails(Employee &emp, QWidget *parent): QWidget(parent), ui(new Ui::EditEmployeeDetails), a_Employee(emp)
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
};

void EditEmployeeDetails::setEmployeeContext()
{  
    ui->nameLineEdit->setText(QString::fromStdString(a_Employee.fullName));
    ui->departmentComboBox->setCurrentIndex(to_int(a_Employee.department));
    ui->positionLineEdit->setText(QString::fromStdString(a_Employee.position));
    ui->jobLevelComboBox->setCurrentIndex(to_int(a_Employee.jobLevel));
    ui->empStatusComboBox->setCurrentIndex(to_int(a_Employee.status));

    ui->dateHiredDateEdit->setDate(QDate::fromString(QString::fromStdString(to_string(a_Employee.dateHired)), "yyyy-MM-dd"));
    ui->DateSeparatedDateEdit->setDate(QDate::fromString(QString::fromStdString(to_string(a_Employee.dateSeparation)), "yyyy-MM-dd"));

    ui->sssNumberLineEdit->setText(QString::fromStdString(a_Employee.sssNumber));
    ui->philHealthNumberLineEdit->setText(QString::fromStdString(a_Employee.philHealthNumber));
    ui->hdmfNumberLineEdit->setText(QString::fromStdString(a_Employee.hdmfNumber));
    ui->tinLineEdit->setText(QString::fromStdString(a_Employee.tin));
    ui->bankAccountNumberLineEdit->setText(QString::fromStdString(a_Employee.bankAccountNumber));
    
    ui->monthlySalarySpinBox->setValue(a_Employee.monthlyBasicSalary);
    ui->monthlyAllowancesSpinBox->setValue(a_Employee.monthlyAllowances);
    
    ui->personEmailLineEdit->setText(QString::fromStdString(a_Employee.personalEmail));
    ui->personalMobileNumberLineEdit->setText(QString::fromStdString(a_Employee.personalMobileNumber));
    ui->activeStatusCheckBox->setChecked(a_Employee.isActive);

};

EditEmployeeDetails::~EditEmployeeDetails()
{
    delete ui;
};

