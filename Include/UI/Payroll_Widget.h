#ifndef PAYROLLWIDGET_H
#define PAYROLLWIDGET_H

#include <QDialog>
#include <QVariant>
#include <QWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QSqlDatabase>
#include <QListWidget>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <sqlite3.h>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"
#include "UI/Premium_Values_Widget.h"
#include "UI/Results_Widget.h"
#include "UI/Employee_Validation_Widget.h"


namespace Ui{
class PayrollWidget;
}

class PayrollWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit PayrollWidget(QWidget *parent = nullptr);
    ~PayrollWidget();                

private slots:
    void previousWindow();
    void nextWindow();
signals:


private:
    Ui::PayrollWidget *ui;
    EmployeeValidationWidget* employeeValidation;
    PremiumValuesWidget* premiumValue;
    ResultsWidget* results;
    std::vector<PayrollCalculationResults> payrollResults;
    std::vector<PayrollCalculationResults>* payrollResults_ptr = nullptr;
};

#endif