#include "include/Generated/ui_annualized_tax_reconciliation.h"
#include "UI/Reports-Logs/Annualized_Tax_Reconciliation.h"

AnnualTaxWidget::AnnualTaxWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::AnnualTaxWidget)
{
    ui->setupUi(this);
    taxReports = AppContext::instance().taxReconciliationService().getReconciliation(2025);
    taxReportsPtr = &taxReports;

    model = new TaxReconciliationReportModel(this, taxReportsPtr);
    m_proxy = new TaxReconciliationReportFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->annumTaxTableView->setModel(m_proxy);
    ui->annumTaxTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->annumTaxTableView->setSortingEnabled(true);

    connect(ui->employeeSearchFilter, &QLineEdit::textChanged, this, &AnnualTaxWidget::onEmployeeSearchChanged);
    connect(ui->exportCSVButton, &QPushButton::clicked, this, &AnnualTaxWidget::onExportCSVClicked);
}
AnnualTaxWidget::~AnnualTaxWidget() { delete ui; }

void AnnualTaxWidget::onEmployeeSearchChanged(const QString &filter)
{
    m_proxy->setEmployeeFilter(filter);
}

void AnnualTaxWidget::onExportCSVClicked()
{
}
