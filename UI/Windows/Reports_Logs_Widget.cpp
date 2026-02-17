#include "include/Generated/ui_reports_logs_widget.h"
#include "UI/Reports_Logs_Widget.h"

ReportsLogsWidget::ReportsLogsWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::ReportsLogsWidget),
                                                        annualTaxWidget{},
                                                        budgetUtilWidget{},
                                                        governRemittanceWidget{},
                                                        journalEntryWidget{},
                                                        payrollRegisterWidget{},
                                                        yearEndBenefitWidget{}
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);

    annualTaxWidget = new AnnualTaxWidget(ui->annualTaxTab);
    budgetUtilWidget = new BudgetUtilWidget(ui->budgetTab);
    governRemittanceWidget = new GovernRemittanceWidget(ui->govRemittancesTab);
    journalEntryWidget = new JournalEntryWidget(ui->journalEntriesTab);
    payrollRegisterWidget = new PayrollRegisterWidget(ui->payrollRegistersTab);
    yearEndBenefitWidget = new YearEndBenefitWidget(ui->yearEndBenefitsTab);

    ui->annualTaxTab->layout()->addWidget(annualTaxWidget);
    ui->budgetTab->layout()->addWidget(budgetUtilWidget);
    ui->govRemittancesTab->layout()->addWidget(governRemittanceWidget);
    ui->journalEntriesTab->layout()->addWidget(journalEntryWidget);
    ui->payrollRegistersTab->layout()->addWidget(payrollRegisterWidget);
    ui->yearEndBenefitsTab->layout()->addWidget(yearEndBenefitWidget);
}
ReportsLogsWidget::~ReportsLogsWidget()
{
}