#ifndef BUDGETUTILWIDGET_H
#define BUDGETUTILWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
namespace Ui
{
    class BudgetUtilWidget;
}
class BudgetUtilWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    BudgetUtilWidget(QWidget *parent = nullptr);
    ~BudgetUtilWidget();
    BudgetUtilWidget(const BudgetUtilWidget &) = delete;
    BudgetUtilWidget &operator=(const BudgetUtilWidget &) = delete;

private:
    Ui::BudgetUtilWidget *ui;
    QStringList payPeriodStringList;
    QStringList departmentStringList;
    std::vector<MonthlyBudgetUtilizationReport> reports;
    std::vector<MonthlyBudgetUtilizationReport> *reportsPtr = nullptr;

    MonthlyBudgetUtilizationReportModel *model;
    MonthlyBudgetUtilizationReportFilterProxyModel *m_proxy;
};
#endif