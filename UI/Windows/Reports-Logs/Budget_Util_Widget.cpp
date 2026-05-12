#include "include/Generated/ui_budget_utilization.h"
#include "UI/Reports-Logs/Budget_Util_Widget.h"

BudgetUtilWidget::BudgetUtilWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::BudgetUtilWidget)
{
    ui->setupUi(this);
    reports = AppContext::instance().budgetUtilService().getMonthlyBudget();
    reportsPtr = &reports;

    for (const auto &r : *reportsPtr)
    {
        departmentStringList << QString::fromStdString(r.departmentName);
        payPeriodStringList << QString::fromStdString(r.periodLabel);
    }

    payPeriodStringList.removeDuplicates();
    departmentStringList.removeDuplicates();
    ui->departmentFilter->addItems(departmentStringList);
    ui->payrollPeriodFilter->addItems(payPeriodStringList);
    // budgetTableView
    model = new MonthlyBudgetUtilizationReportModel(this, reportsPtr);
    m_proxy = new MonthlyBudgetUtilizationReportFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->budgetTableView->setModel(m_proxy);
    ui->budgetTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->budgetTableView->setSortingEnabled(true);

    connect(ui->payrollPeriodFilter, &QComboBox::currentIndexChanged, this, &BudgetUtilWidget::onPayPeriodChanged);
    connect(ui->departmentFilter, &QComboBox::currentIndexChanged, this, &BudgetUtilWidget::onDepartmentChanged);
    connect(ui->resetButton, &QPushButton::clicked, this, &BudgetUtilWidget::onResetClicked);
    connect(ui->exportCSVButton, &QPushButton::clicked, this, &BudgetUtilWidget::onExportCSVClicked);
}
BudgetUtilWidget::~BudgetUtilWidget() { delete ui; }

void BudgetUtilWidget::onPayPeriodChanged()
{
    QString selected = ui->payrollPeriodFilter->currentText();
    m_proxy->setPeriodFilter(selected);
}
void BudgetUtilWidget::onDepartmentChanged()
{
    QString department = ui->departmentFilter->currentText();
    m_proxy->setDepartmentFilter(department);
}
void BudgetUtilWidget::onResetClicked()
{
    ui->payrollPeriodFilter->blockSignals(true);
    ui->payrollPeriodFilter->setCurrentIndex(-1);
    ui->payrollPeriodFilter->blockSignals(false);

    ui->departmentFilter->blockSignals(true);
    ui->departmentFilter->setCurrentIndex(-1);
    ui->departmentFilter->blockSignals(false);

    m_proxy->setPeriodFilter("");
    m_proxy->setDepartmentFilter("");
}
void BudgetUtilWidget::onExportCSVClicked()
{
}
