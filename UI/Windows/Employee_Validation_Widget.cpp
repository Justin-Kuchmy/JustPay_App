#include "include/Generated/ui_employee_validation_widget.h"
#include "UI/Employee_Validation_Widget.h"
#include "Services/AppContext.h"

EmployeeValidationWidget::EmployeeValidationWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::EmployeeValidationWidget), dataBus(dataBus)
{
    ui->setupUi(this);
    connect(ui->loadButton, &QPushButton::clicked, this, &EmployeeValidationWidget::loadEmployees);

    
    
}
EmployeeValidationWidget::~EmployeeValidationWidget()
{
    delete ui;
    delete dataBus;
}

void EmployeeValidationWidget::loadEmployees()
{
    this->m_Employees = AppContext::instance().employeeService().getAllEmployees();
    ui->employeeTableWidget->setRowCount(m_Employees.size());
    //ui->employeeTableWidget->setColumnCount(5);

    for(size_t i{}; i < m_Employees.size(); i++)
    {
        Employee empName = m_Employees.at(i);
        QTableWidgetItem* checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        checkItem->setCheckState(Qt::Checked);
        ui->employeeTableWidget->setItem(i, 0, checkItem);
        ui->employeeTableWidget->setItem(i, 1,  new QTableWidgetItem(QString::fromStdString(empName.employeeId)));
        ui->employeeTableWidget->setItem(i, 2,  new QTableWidgetItem(QString::fromStdString(empName.fullName)));
        ui->employeeTableWidget->setItem(i, 3,  new QTableWidgetItem(QString::fromStdString(department_to_string(empName.department))));
        ui->employeeTableWidget->setItem(i, 4,  new QTableWidgetItem(QString::fromStdString(empName.isActive? "Active" : "Inactive")));
        
    }
    emit dataLoaded();
}