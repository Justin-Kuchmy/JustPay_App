#ifndef REPORTSLOGSWIDGET_H
#define REPORTSLOGSWIDGET_H

#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"
#include "UI/Annualized_Tax_Reconciliation.h"
#include "UI/Budget_Util_Widget.h"
#include "UI/Govern_Remittance_Widget.h"
#include "UI/Journal_Entry_Widget.h"
#include "UI/Payroll_Register_Widget.h"
#include "UI/YearEnd_Benefit_Widget.h"

namespace Ui
{
    class ReportsLogsWidget;
}

class ReportsLogsWidget : public BaseContentWidget
{
    Q_OBJECT

public:
    explicit ReportsLogsWidget(QWidget *parent = nullptr);
    ~ReportsLogsWidget();
    ReportsLogsWidget(const ReportsLogsWidget &) = delete;
    ReportsLogsWidget &operator=(const ReportsLogsWidget &) = delete;

private:
    Ui::ReportsLogsWidget *ui;
    AnnualTaxWidget *annualTaxWidget;
    BudgetUtilWidget *budgetUtilWidget;
    GovernRemittanceWidget *governRemittanceWidget;
    JournalEntryWidget *journalEntryWidget;
    PayrollRegisterWidget *payrollRegisterWidget;
    YearEndBenefitWidget *yearEndBenefitWidget;
};

#endif