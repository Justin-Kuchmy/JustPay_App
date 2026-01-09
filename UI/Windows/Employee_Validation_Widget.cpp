#include "include/Generated/ui_employee_validation_widget.h"
#include "UI/Employee_Validation_Widget.h"
#include "Services/AppContext.h"

constexpr int MONTHS_PER_YEAR = 12;
constexpr int WORKING_DAYS_PER_YEAR = 314;
constexpr int WORKING_HOURS_PER_DAY = 8;
EmployeeValidationWidget::EmployeeValidationWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::EmployeeValidationWidget),
dataBus(dataBus),
m_Employees{}
{
    ui->setupUi(this);
    loadEmployees();
    connect(ui->loadButton, &QPushButton::clicked, this, &EmployeeValidationWidget::loadDataBus);

    
    
}
EmployeeValidationWidget::~EmployeeValidationWidget()
{
    delete ui;
}


void EmployeeValidationWidget::loadDataBus()
{
    if(dataBus->size() > 0)
    {
        dataBus->clear();
    }
    const auto& rows = ui->employeeTableWidget->rowCount();
    for(int i{}; i < rows; i++)
    {
        if(ui->employeeTableWidget->item(i, 0)->checkState() == Qt::Checked)
        {
            PayrollCalculationResults data{};
            
            data.employeeId = ui->employeeTableWidget->item(i, 1)->text().toStdString();
            data.fullName = ui->employeeTableWidget->item(i, 2)->text().toStdString();
            data.employeeDepartment = ui->employeeTableWidget->item(i, 3)->text().toStdString();

            auto optEmp = AppContext::instance().employeeService().getEmployeeByID(data.employeeId);
            if(optEmp.has_value())
            {
                Employee emp = optEmp.value(); 
                data.monthlyBasicSalary =  emp.monthlyBasicSalary;
                data.monthlyAllowances = emp.monthlyAllowances;
            }
            std::vector<AttendanceLog> empOverTime = AppContext::instance().attendanceLogService().getAllAttendanceLogsById(data.employeeId);
            for(size_t i{}; i < empOverTime.size(); i++)
            {

                const double hrRate{data.monthlyBasicSalary * MONTHS_PER_YEAR / WORKING_DAYS_PER_YEAR / WORKING_HOURS_PER_DAY};

                const double pay{empOverTime.at(i).getOvertimePay()};

                const double otPay{std::floor(pay * hrRate * 100.0) / 100.0};
                data.overTimePay += otPay;
            }
            
           
            dataBus->push_back(data);
            LOG_DEBUG("added employee: " << data.employeeId);
        }
        
    }
    LOG_DEBUG("\n"); 
    emit dataLoaded();
    
}


void EmployeeValidationWidget::loadEmployees()
{
    this->m_Employees = AppContext::instance().employeeService().getAllEmployees();
    ui->employeeTableWidget->setRowCount(static_cast<int>(m_Employees.size()));

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

}