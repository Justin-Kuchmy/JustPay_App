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

private:
    Ui::BudgetUtilWidget *ui;
};
#endif