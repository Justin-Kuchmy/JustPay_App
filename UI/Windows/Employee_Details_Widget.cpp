#include "include/Generated/ui_employee_details_widget.h"
#include "UI/Employee_Details_Widget.h"



EmployeeDetailsWidget::EmployeeDetailsWidget(QWidget *parent): BaseContentWidget(parent), ui(new Ui::EmployeeDetailsWidget),
m_Employee{},
employees{},
editEmployeeForm{},
editContactForm{},
editDependentForm{},
loanLedgetWidget{},
employeeAttendanceLogWidget{},
optContact{},
optDependent{},
optLoanLedgers{},
optAttendanceLogs{}
{
    ui->setupUi(this); 
    ui->mainSplitter->setSizes({300, 900});
    employees = new std::vector<Employee>(AppContext::instance().employeeService().getAllEmployees());

    refreshEmployeeList();

    connect(ui->employeeSearchBox, &QLineEdit::textChanged,this, &EmployeeDetailsWidget::onEmployeeSearchBoxChanged);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    //connect(ui->backButton, &QPushButton::clicked, this, [this]() {emit backRequested();});

    editEmployeeForm = new EditEmployeeDetails(ui->tabGeneral);
    loanLedgetWidget = new LoanLedgerWidget(ui->tabLoans);
    editContactForm = new EditEmergencyContactDetails(ui->tabContacts);
    editDependentForm = new EditDependentDetails(ui->tabDependent);
    employeeAttendanceLogWidget = new EmployeeAttendanceWidget(ui->tabAttendance);
    
    QVBoxLayout* generalLayout = new QVBoxLayout(ui->tabGeneral);
    QVBoxLayout* loanLayout = new QVBoxLayout(ui->tabLoans);
    QVBoxLayout* contactLayout = new QVBoxLayout(ui->tabContacts);
    QVBoxLayout* dependentLayout = new QVBoxLayout(ui->tabDependent);
    QVBoxLayout* attendanceLayout = new QVBoxLayout(ui->tabAttendance);
    
    generalLayout->setContentsMargins(0, 0, 0, 0);
    loanLayout->setContentsMargins(0, 0, 0, 0);
    contactLayout->setContentsMargins(0, 0, 0, 0);
    dependentLayout->setContentsMargins(0, 0, 0, 0);
    attendanceLayout->setContentsMargins(0, 0, 0, 0);
    
    generalLayout->addWidget(editEmployeeForm);
    loanLayout->addWidget(loanLedgetWidget);
    contactLayout->addWidget(editContactForm);
    dependentLayout->addWidget(editDependentForm);
    attendanceLayout->addWidget(employeeAttendanceLogWidget);



};

EmployeeDetailsWidget::~EmployeeDetailsWidget() 
{
    delete employees;
    delete ui;
};
void EmployeeDetailsWidget::onEmployeeSearchBoxChanged(const QString& text)
{
    refreshEmployeeList(text);
};

bool EmployeeDetailsWidget::loadEmployeeDetails(QString& empID)
{
    auto optEmployee = AppContext::instance().employeeService().getEmployeeByID(empID.toStdString());

    if(optEmployee)
    {
        m_Employee = optEmployee.value();
        editEmployeeForm->setEmployeeContext(m_Employee);
        return true;
    }
    return false;
}

void EmployeeDetailsWidget::refreshEmployeeList(const QString &filter)
{
    QLayout *layout = ui->employeeListContainer->layout();
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    // Repopulate
    for (const auto &emp : *employees) {
        QString name = QString::fromUtf8(emp.fullName);
        if (filter.isEmpty() || name.contains(filter, Qt::CaseInsensitive)) {
            QPushButton *employeeButton = new QPushButton(name);
            employeeButton->setCursor(Qt::PointingHandCursor);
            employeeButton->setProperty("employeeId", QString::fromStdString(emp.employeeId));
            layout->addWidget(employeeButton);

            connect(employeeButton, &QPushButton::clicked, this, [this, employeeButton]() {
            QString empId = employeeButton->property("employeeId").toString();
            this->onEmployeeSelected(empId);
            
        });
        }

    }
};

void EmployeeDetailsWidget::onEmployeeSelected(QString& empID)
{
    if(EmployeeDetailsWidget::loadEmployeeDetails(empID))
    {
        ui->employeeNameLabel->setText(QString::fromStdString(m_Employee.fullName));
        this->optContact = AppContext::instance().emergencyContactService().getEmergencyContactByID(m_Employee.contactId);
        this->optDependent = AppContext::instance().dependentService().getDependentByID(m_Employee.dependentId);
        this->optLoanLedgers = AppContext::instance().loanLedgerService().getAllLoanLedgers(m_Employee.employeeId);
        this->optAttendanceLogs = AppContext::instance().attendanceLogService().getAllAttendanceLogsById(m_Employee.employeeId);
        if(optContact)    
        {
            editContactForm->setContactContext(optContact);
        }
        else
        {
            LOG_DEBUG("No contact data available");
            return;
        }
        if(optDependent)
        {
            editDependentForm->setDependentContext(optDependent);
        }
        else
        {
            LOG_DEBUG("No dependent data available");
            return;
        }
        loanLedgetWidget->setLoanLedgerContext(&optLoanLedgers, m_Employee.employeeId);
        employeeAttendanceLogWidget->setEmployeeAttendanceContext(&optAttendanceLogs, m_Employee.employeeId);
    }
    else
    {
        LOG_DEBUG("Employee not found:" << empID.toStdString());
    }


};

QString EmployeeDetailsWidget::getSelectedEmployee() const
{
    return QString::fromStdString(m_Employee.fullName);
};
