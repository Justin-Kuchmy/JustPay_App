#ifndef PAYROLLREGISTERWIDGET_H
#define PAYROLLREGISTERWIDGET_H
#include <QWidget>
#include <QDate>
#include "Utils/BaseContentWidget.h"
#include "Models/DataObjects.h"
#include "Models/QObjects.h"
#include <QSortFilterProxyModel>

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
    void onApplyClicked();
    void onResetClicked();
    void onPayrollPeriodChanged();
    void onDepartmentChanged();

private:
    Ui::PayrollRegisterWidget *ui;
    std::vector<PayrollCalculationResults> payrollVec;
    QStringList payrollByMonth;
    QStringList payrollByfortnight;
    QStringList departments;
    PayrollRegisterModel *model;
    PayrollFilterProxyModel *m_proxy;
};
#endif