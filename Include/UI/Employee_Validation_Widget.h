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

private slots:
    void loadEmployees();
signals: 
    void dataLoaded();
private:
    Ui::EmployeeValidationWidget *ui;
    std::vector<Employee> m_Employees;
    std::vector<PayrollCalculationResults>* dataBus = nullptr;
};

#endif