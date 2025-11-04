#include "include/Generated/ui_employee_details_widget.h"
#include "UI/Employee_Details_Widget.h"



EmployeeDetailsWidget::EmployeeDetailsWidget(QWidget *parent): BaseContentWidget(parent), ui(new Ui::EmployeeDetailsWidget)
{
    ui->setupUi(this); 
    ui->mainSplitter->setSizes({300, 900});
    employees = new std::vector<Employee>(AppContext::instance().employeeService().getAllEmployees());

    refreshEmployeeList();

    connect(ui->employeeSearchBox, &QLineEdit::textChanged,this, &EmployeeDetailsWidget::onEmployeeSearchBoxChanged);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    //connect(ui->backButton, &QPushButton::clicked, this, [this]() {emit backRequested();});

    editEmployeeForm = new EditEmployeeDetails(a_Employee, ui->tabGeneral);
    editContactForm = new EditEmergencyContactDetails(a_Employee, ui->tabContacts);
    editDependentForm = new EditDependentDetails(a_Employee, ui->tabDependent);
    loanLedgetWidget = new LoanLedgerWidget(ui->tabLoans);

    QVBoxLayout* generalLayout = new QVBoxLayout(ui->tabGeneral);
    QVBoxLayout* contactLayout = new QVBoxLayout(ui->tabContacts);
    QVBoxLayout* dependentLayout = new QVBoxLayout(ui->tabDependent);
    QVBoxLayout* loanLayout = new QVBoxLayout(ui->tabLoans);

    generalLayout->setContentsMargins(0, 0, 0, 0);
    contactLayout->setContentsMargins(0, 0, 0, 0);
    dependentLayout->setContentsMargins(0, 0, 0, 0);
    loanLayout->setContentsMargins(0, 0, 0, 0);

    generalLayout->addWidget(editEmployeeForm);
    contactLayout->addWidget(editContactForm);
    dependentLayout->addWidget(editDependentForm);
    loanLayout->addWidget(loanLedgetWidget);

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

void EmployeeDetailsWidget::loadEmployeeDetails()
{
    editEmployeeForm->setEmployeeContext();
    editContactForm->setEmployeeContext();
    editDependentForm->setEmployeeContext();
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
            this->loadEmployeeDetails();
        });
        }

    }
};

void EmployeeDetailsWidget::onEmployeeSelected(QString& empID)
{
    auto optEmployee = AppContext::instance().employeeService().getEmployeeByID(empID.toStdString());
    auto optContact = AppContext::instance().emergencyContactService().getEmergencyContactByID(empID.toStdString());
    auto optDependent = AppContext::instance().dependentService().getDependentByID(empID.toStdString());
    if(optEmployee.has_value())
    {
        a_Employee = *optEmployee;
        if(optContact.has_value()) a_Employee.emergencyContact = *optContact;
        if(optDependent.has_value()) a_Employee.dependent = *optDependent;
    }
    else
    {
        LOG_DEBUG("Employee not found:" << empID.toStdString());
    }

};

QString EmployeeDetailsWidget::getSelectedEmployee() const
{
    return QString::fromStdString(a_Employee.fullName);
};
