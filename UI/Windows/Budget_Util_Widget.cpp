#include "include/Generated/ui_budget_utilization.h"
#include "UI/Budget_Util_Widget.h"

BudgetUtilWidget::BudgetUtilWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::BudgetUtilWidget)
{
    ui->setupUi(this);
}
BudgetUtilWidget::~BudgetUtilWidget() { delete ui; }
