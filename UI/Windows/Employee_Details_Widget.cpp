#include "include/Generated/ui_employee_details_widget.h"
#include "UI/Employee_Details_Widget.h"



EmployeeDetailsWidget::EmployeeDetailsWidget(QWidget *parent): QWidget(parent), ui(new Ui::EmployeeDetailsWidget)
{
    ui->setupUi(this); 
    ui->mainSplitter->setSizes({300, 900});
    employees = new std::vector<Employee>(AppContext::instance().employeeService().getAllEmployees());

    refreshEmployeeList();

    connect(ui->employeeSearchBox, &QLineEdit::textChanged,this, &EmployeeDetailsWidget::onEmployeeSearchBoxChanged);

    editEmployeeForm = new EditEmployeeDetails(a_Employee, ui->tabGeneral);
    editContactForm = new EditEmergencyContactDetails(a_Employee, ui->tabContacts);
    editDependentForm = new EditDependentDetails(a_Employee, ui->tabDependent);

    QVBoxLayout* generalLayout = new QVBoxLayout(ui->tabGeneral);
    QVBoxLayout* contactLayout = new QVBoxLayout(ui->tabContacts);
    QVBoxLayout* dependentLayout = new QVBoxLayout(ui->tabDependent);

    generalLayout->setContentsMargins(0, 0, 0, 0);
    contactLayout->setContentsMargins(0, 0, 0, 0);
    dependentLayout->setContentsMargins(0, 0, 0, 0);

    generalLayout->addWidget(editEmployeeForm);
    contactLayout->addWidget(editContactForm);
    dependentLayout->addWidget(editDependentForm);

};

EmployeeDetailsWidget::~EmployeeDetailsWidget() 
{
    delete employees;
    delete ui;
};
void EmployeeDetailsWidget::onOKClicked()
{
    
};
void EmployeeDetailsWidget::onCancelClicked()
{
    
};
void EmployeeDetailsWidget::openEditContactDialog()
{
    
};
void EmployeeDetailsWidget::openEditDependentDialog()
{
    
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
    if(optEmployee.has_value())
    {
        a_Employee = *optEmployee;
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
