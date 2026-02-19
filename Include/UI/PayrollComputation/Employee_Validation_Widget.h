#ifndef EMPLOYEEVALIDATIONWIDGET_H
#define EMPLOYEEVALIDATIONWIDGET_H
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"
#include <vector>
#include <QTableWidget>
#include <QCheckBox>
#include <QTableWidgetItem>

namespace Ui{
class EmployeeValidationWidget;
}

class EmployeeValidationWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit EmployeeValidationWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent = nullptr);
    ~EmployeeValidationWidget();
    EmployeeValidationWidget(const EmployeeValidationWidget&) = delete; 
    EmployeeValidationWidget& operator=(const EmployeeValidationWidget&) = delete;    
    void loadDataBus();                

private slots:
    void loadEmployees();
signals: 
    void dataLoaded();
private:
    Ui::EmployeeValidationWidget *ui;
    std::vector<PayrollCalculationResults>* dataBus = nullptr;
    std::vector<Employee> m_Employees;
};

#endif