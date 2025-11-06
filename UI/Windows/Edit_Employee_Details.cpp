#include "include/Generated/ui_edit_employee_details.h"
#include "UI/Edit_Employee_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#define DEBUG_LOGS
#include "Utils/Log.h"



EditEmployeeDetails::EditEmployeeDetails(QWidget *parent): QWidget(parent), ui(new Ui::EditEmployeeDetails)
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

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditEmployeeDetails::onSaveClicked);
};

void EditEmployeeDetails::setEmployeeContext(Employee &emp)
{  
    this-> m_Employee = std::make_unique<Employee>(emp);
    ui->nameLineEdit->setText(QString::fromStdString(m_Employee->fullName));
    ui->departmentComboBox->setCurrentIndex(to_int(m_Employee->department));
    ui->positionLineEdit->setText(QString::fromStdString(m_Employee->position));
    ui->jobLevelComboBox->setCurrentIndex(to_int(m_Employee->jobLevel));
    ui->empStatusComboBox->setCurrentIndex(to_int(m_Employee->status));
    
    ui->dateHiredDateEdit->setDate(QDate::fromString(QString::fromStdString(to_string(m_Employee->dateHired)), "yyyy-MM-dd"));
    ui->DateSeparatedDateEdit->setDate(QDate::fromString(QString::fromStdString(to_string(m_Employee->dateSeparation)), "yyyy-MM-dd"));
    
    ui->sssNumberLineEdit->setText(QString::fromStdString(m_Employee->sssNumber));
    ui->philHealthNumberLineEdit->setText(QString::fromStdString(m_Employee->philHealthNumber));
    ui->hdmfNumberLineEdit->setText(QString::fromStdString(m_Employee->hdmfNumber));
    ui->tinLineEdit->setText(QString::fromStdString(m_Employee->tin));
    ui->bankAccountNumberLineEdit->setText(QString::fromStdString(m_Employee->bankAccountNumber));
    
    ui->monthlySalarySpinBox->setValue(m_Employee->monthlyBasicSalary);
    ui->monthlyAllowancesSpinBox->setValue(m_Employee->monthlyAllowances);
    
    ui->personEmailLineEdit->setText(QString::fromStdString(m_Employee->personalEmail));
    ui->personalMobileNumberLineEdit->setText(QString::fromStdString(m_Employee->personalMobileNumber));
    ui->activeStatusCheckBox->setChecked(m_Employee->isActive);

};

EditEmployeeDetails::~EditEmployeeDetails()
{
    delete ui;
};

void EditEmployeeDetails::onSaveClicked()
{
    m_Employee->fullName = ui->nameLineEdit->text().toStdString();
    m_Employee->department = to_department(ui->departmentComboBox->currentIndex());
    m_Employee->position = ui->positionLineEdit->text().toStdString();
    m_Employee->jobLevel = to_jobLevel(ui->jobLevelComboBox->currentIndex());
    m_Employee->status = to_status(ui->empStatusComboBox->currentIndex());
    m_Employee->dateHired = from_string(ui->dateHiredDateEdit->date().toString("yyyy-MM-dd").toStdString());
    m_Employee->dateSeparation = from_string(ui->DateSeparatedDateEdit->date().toString("yyyy-MM-dd").toStdString());
    m_Employee->sssNumber = ui->sssNumberLineEdit->text().toStdString();
    m_Employee->philHealthNumber = ui->philHealthNumberLineEdit->text().toStdString();
    m_Employee->hdmfNumber = ui->hdmfNumberLineEdit->text().toStdString();
    m_Employee->tin = ui->tinLineEdit->text().toStdString();
    m_Employee->bankAccountNumber = ui->bankAccountNumberLineEdit->text().toStdString();
    m_Employee->monthlyBasicSalary = ui->monthlySalarySpinBox->value();
    m_Employee->monthlyAllowances = ui->monthlyAllowancesSpinBox->value();
    m_Employee->personalEmail = ui->personEmailLineEdit->text().toStdString();
    m_Employee->personalMobileNumber = ui->personalMobileNumberLineEdit->text().toStdString();
    m_Employee->isActive = ui->activeStatusCheckBox->isChecked();

    if(AppContext::instance().employeeService().updateEmployee(*m_Employee))
    {
        LOG_DEBUG(m_Employee->fullName << " updated!");
    }
};
