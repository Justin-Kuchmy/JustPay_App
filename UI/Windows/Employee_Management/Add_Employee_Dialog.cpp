#include "include/Generated/ui_add_employee_dialog.h"
#include "UI/Employee_Management/Add_Employee_Dialog.h"
#include "UI/Employee_Management/Add_Dependent_Dialog.h"
#include "UI/Employee_Management/Add_Emergency_Contact_Dialog.h"
#include <QWidget>
#include <QListWidgetItem>
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Utils/DialogFactory.h"
#include "Services/AppContext.h"

AddEmployeeDialog::AddEmployeeDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddEmployeeDialog),
                                                        employees{}, a_Employee{}, m_Contact{}, m_Dependent{}
{
    ui->setupUi(this);

    ui->departmentComboBox->addItems({QString::fromStdString(department_to_string(Department::HR)),
                                      QString::fromStdString(department_to_string(Department::Finance)),
                                      QString::fromStdString(department_to_string(Department::IT)),
                                      QString::fromStdString(department_to_string(Department::Operations)),
                                      QString::fromStdString(department_to_string(Department::Sales)),
                                      QString::fromStdString(department_to_string(Department::Marketing)),
                                      QString::fromStdString(department_to_string(Department::Admin)),
                                      QString::fromStdString(department_to_string(Department::Legal)),
                                      QString::fromStdString(department_to_string(Department::Engineering))}); // Department enum
    ui->jobLevelComboBox->addItems({QString::fromStdString(joblevel_to_string(JobLevel::RankAndFile)),
                                    QString::fromStdString(joblevel_to_string(JobLevel::Supervisory)),
                                    QString::fromStdString(joblevel_to_string(JobLevel::Managerial)),
                                    QString::fromStdString(joblevel_to_string(JobLevel::Executive)),
                                    QString::fromStdString(joblevel_to_string(JobLevel::BoardOwnership))}); // JobLevel enum
    ui->empStatusComboBox->addItems({QString::fromStdString(status_to_string(EmploymentStatus::Regular)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::Probationary)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::Contractual)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::ProjectBased)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::Seasonal)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::Casual)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::PartTime)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::InternOJT)),
                                     QString::fromStdString(status_to_string(EmploymentStatus::Consultant))}); // EmploymentStatus enum

    ui->monthlySalarySpinBox->setRange(0.00, 1'000'000'000.00); // 1 billion max
    ui->monthlySalarySpinBox->setDecimals(2);
    // ui->monthlySalarySpinBox->setPrefix("₱ ");
    ui->monthlySalarySpinBox->setSingleStep(1000.00);

    ui->monthlyAllowancesSpinBox->setRange(0.00, 1'000'000'000.00); // 1 billion max
    ui->monthlyAllowancesSpinBox->setDecimals(2);
    // ui->monthlyAllowancesSpinBox->setPrefix("₱ ");
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
    auto contactDialog = dynamic_cast<EmergencyContactDialog *>(dlg);
    if (contactDialog && contactDialog->exec() == QDialog::Accepted)
        m_Contact = contactDialog->getContactData();
};

void AddEmployeeDialog::openAddDependentDialog()
{
    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("add_dependent");
    auto dependentDialog = dynamic_cast<AddDependentDialog *>(dlg);
    if (dependentDialog && dependentDialog->exec() == QDialog::Accepted)
        m_Dependent = dependentDialog->getDependentData();
};

void AddEmployeeDialog::onOKClicked()
{
    int contactID = AppContext::instance().emergencyContactService().addEmergencyContact(this->m_Contact);
    int dependentID = AppContext::instance().dependentService().addDependent(this->m_Dependent);

    a_Employee.fullName = ui->nameLineEdit->text().toStdString();
    // a_Employee.employeeId; //this will be auto generated
    a_Employee.department = static_cast<Department>(ui->departmentComboBox->currentIndex());
    a_Employee.position = ui->positionLineEdit->text().toStdString();
    a_Employee.jobLevel = static_cast<JobLevel>(ui->jobLevelComboBox->currentIndex());
    a_Employee.status = static_cast<EmploymentStatus>(ui->empStatusComboBox->currentIndex());
    a_Employee.dateHired = Date::fromString(ui->dateHiredDateEdit->date().toString("yyyy-MM-dd").toStdString());
    a_Employee.dateSeparation = Date::fromString(ui->DateSeparatedDateEdit->date().toString("yyyy-MM-dd").toStdString());
    a_Employee.sssNumber = ui->sssNumberLineEdit->text().toStdString();
    a_Employee.philHealthNumber = ui->philHealthNumberLineEdit->text().toStdString();
    a_Employee.hdmfNumber = ui->hdmfNumberLineEdit->text().toStdString();
    a_Employee.tin = ui->tinLineEdit->text().toStdString();
    a_Employee.clockInTimeStr = ui->clockInTimeEdit->time().toString("HH:mm").toStdString();
    a_Employee.clockOutTimeStr = ui->clockOutTimeEdit->time().toString("HH:mm").toStdString();
    a_Employee.bankAccountNumber = ui->bankAccountNumberLineEdit->text().toStdString();
    a_Employee.monthlyBasicSalary = ui->monthlySalarySpinBox->value();
    a_Employee.monthlyAllowances = ui->monthlyAllowancesSpinBox->value();
    a_Employee.personalEmail = ui->personEmailLineEdit->text().toStdString();
    a_Employee.personalMobileNumber = ui->personalMobileNumberLineEdit->text().toStdString();
    a_Employee.isActive = ui->activeStatusCheckBox->isChecked();
    if (contactID > 0 && dependentID > 0)
    {
        this->a_Employee.contactId = contactID;
        this->a_Employee.dependentId = dependentID;
        if (!AppContext::instance().employeeService().addEmployee(a_Employee).empty())
        {
            accepted();
        }
    }
};

void AddEmployeeDialog::onCancelClicked()
{
    rejected();
};