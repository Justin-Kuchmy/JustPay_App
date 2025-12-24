#include "include/Generated/ui_employee_validation_widget.h"
#include "UI/Employee_Validation_Widget.h"
#include "Services/AppContext.h"

EmployeeValidationWidget::EmployeeValidationWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::EmployeeValidationWidget),
dataBus(dataBus),
m_Employees{}
{
    ui->setupUi(this);
    connect(ui->loadButton, &QPushButton::clicked, this, &EmployeeValidationWidget::loadEmployees);

    
    
}
EmployeeValidationWidget::~EmployeeValidationWidget()
{
    delete ui;
}

void EmployeeValidationWidget::loadEmployees()
{
    this->m_Employees = AppContext::instance().employeeService().getAllEmployees();
    ui->employeeTableWidget->setRowCount(static_cast<int>(m_Employees.size()));
    //ui->employeeTableWidget->setColumnCount(5);

    for(size_t i{}; i < m_Employees.size(); i++)
    {
        Employee empName = m_Employees.at(i);
        QTableWidgetItem* checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        checkItem->setCheckState(Qt::Checked);
        ui->employeeTableWidget->setItem(static_cast<int>(i), 0, checkItem);
        ui->employeeTableWidget->setItem(static_cast<int>(i), 1,  new QTableWidgetItem(QString::fromStdString(empName.employeeId)));
        ui->employeeTableWidget->setItem(static_cast<int>(i), 2,  new QTableWidgetItem(QString::fromStdString(empName.fullName)));
        ui->employeeTableWidget->setItem(static_cast<int>(i), 3,  new QTableWidgetItem(QString::fromStdString(department_to_string(empName.department))));
        ui->employeeTableWidget->setItem(static_cast<int>(i), 4,  new QTableWidgetItem(QString::fromStdString(empName.isActive? "Active" : "Inactive")));
        
    }
    emit dataLoaded();
}