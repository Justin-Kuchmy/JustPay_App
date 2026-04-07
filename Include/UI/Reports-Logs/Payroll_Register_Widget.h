#ifndef PAYROLLREGISTERWIDGET_H
#define PAYROLLREGISTERWIDGET_H
#include <QWidget>
#include <QDate>
#include "Utils/BaseContentWidget.h"
#include "Models/payroll.h"
#include "Models/Core/QObjects.h"
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>

namespace Ui
{
    class PayrollRegisterWidget;
}
class PayrollRegisterWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    PayrollRegisterWidget(QWidget *parent = nullptr);
    ~PayrollRegisterWidget();
    void populatePayPeriodDropDownByMonth();
    void populatePayPeriodDropDownByFortNight();

public slots:
    void onGranularityChanged();
    void onEmployeeSearchChanged(const QString &filter);
    void onResetClicked();
    void onPayrollPeriodChanged();
    void onDepartmentChanged();
    void exportCSVClicked();

private:
    Ui::PayrollRegisterWidget *ui;
    std::vector<PayrollCalculationResults> payrollVecMonthly;
    std::vector<PayrollCalculationResults> payrollFortNight;
    std::vector<PayrollCalculationResults> *activePayroll = nullptr;
    QStringList payrollByMonth;
    QStringList payrollByfortnight;
    QStringList departments;
    PayrollRegisterModel *model;
    PayrollFilterProxyModel *m_proxy;
};
#endif