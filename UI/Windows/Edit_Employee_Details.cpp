#include "include/Generated/ui_edit_employee_details.h"
#include "UI/Edit_Employee_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

EditEmployeeDetails::EditEmployeeDetails(QWidget *parent) : QWidget(parent), ui(new Ui::EditEmployeeDetails),
                                                            m_Employee{}
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

    // Make the scroll area resize its inner widget properly
    ui->scrollArea->setWidgetResizable(true);

    // Optional: force vertical scrollbar to appear as needed
    ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditEmployeeDetails::onSaveClicked);
};

void EditEmployeeDetails::setEmployeeContext(Employee &emp)
{
    this->m_Employee = std::make_unique<Employee>(emp);
    ui->nameLineEdit->setText(QString::fromStdString(m_Employee->fullName));
    ui->departmentComboBox->setCurrentIndex(static_cast<int>(m_Employee->department));
    ui->positionLineEdit->setText(QString::fromStdString(m_Employee->position));
    ui->jobLevelComboBox->setCurrentIndex(static_cast<int>(m_Employee->jobLevel));
    ui->empStatusComboBox->setCurrentIndex(static_cast<int>(m_Employee->status));

    ui->dateHiredDateEdit->setDate(QDate::fromString(QString::fromStdString(m_Employee->dateHired.to_string()), "yyyy-MM-dd"));
    ui->DateSeparatedDateEdit->setDate(QDate::fromString(QString::fromStdString(m_Employee->dateSeparation.to_string()), "yyyy-MM-dd"));

    ui->sssNumberLineEdit->setText(QString::fromStdString(m_Employee->sssNumber));
    ui->philHealthNumberLineEdit->setText(QString::fromStdString(m_Employee->philHealthNumber));
    ui->hdmfNumberLineEdit->setText(QString::fromStdString(m_Employee->hdmfNumber));
    ui->tinLineEdit->setText(QString::fromStdString(m_Employee->tin));
    ui->bankAccountNumberLineEdit->setText(QString::fromStdString(m_Employee->bankAccountNumber));
    ui->clockInTimeEdit->setTime(QTime::fromString(QString::fromStdString(m_Employee->clockInTimeStr)));
    ui->clockOutTimeEdit->setTime(QTime::fromString(QString::fromStdString(m_Employee->clockOutTimeStr)));

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
    m_Employee->department = static_cast<Department>(ui->departmentComboBox->currentIndex());
    m_Employee->position = ui->positionLineEdit->text().toStdString();
    m_Employee->jobLevel = static_cast<JobLevel>(ui->jobLevelComboBox->currentIndex());
    m_Employee->status = static_cast<EmploymentStatus>(ui->empStatusComboBox->currentIndex());
    m_Employee->dateHired = Date::fromString(ui->dateHiredDateEdit->date().toString("yyyy-MM-dd").toStdString());
    m_Employee->dateSeparation = Date::fromString(ui->DateSeparatedDateEdit->date().toString("yyyy-MM-dd").toStdString());
    m_Employee->sssNumber = ui->sssNumberLineEdit->text().toStdString();
    m_Employee->philHealthNumber = ui->philHealthNumberLineEdit->text().toStdString();
    m_Employee->hdmfNumber = ui->hdmfNumberLineEdit->text().toStdString();
    m_Employee->tin = ui->tinLineEdit->text().toStdString();
    m_Employee->bankAccountNumber = ui->bankAccountNumberLineEdit->text().toStdString();
    m_Employee->clockInTimeStr = ui->clockInTimeEdit->time().toString("HH:mm").toStdString();
    m_Employee->clockOutTimeStr = ui->clockOutTimeEdit->time().toString("HH:mm").toStdString();
    m_Employee->monthlyBasicSalary = ui->monthlySalarySpinBox->value();
    m_Employee->monthlyAllowances = ui->monthlyAllowancesSpinBox->value();
    m_Employee->personalEmail = ui->personEmailLineEdit->text().toStdString();
    m_Employee->personalMobileNumber = ui->personalMobileNumberLineEdit->text().toStdString();
    m_Employee->isActive = ui->activeStatusCheckBox->isChecked();

    if (AppContext::instance().employeeService().updateEmployee(*m_Employee))
    {
        LOG_DEBUG(m_Employee->fullName << " updated!");
    }
};
