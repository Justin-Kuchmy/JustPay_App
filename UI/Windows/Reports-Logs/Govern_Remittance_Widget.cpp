#include "include/Generated/ui_gov_remittance.h"
#include "UI/Reports-Logs/Govern_Remittance_Widget.h"
#include "Helpers/CsvExporter.h"
GovernRemittanceWidget::GovernRemittanceWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::GovernRemittanceWidget)
{
    ui->setupUi(this);
    remits = AppContext::instance().governmentRemittanceService().getAll();
    remitsPtr = &remits;

    for (auto &v : *remitsPtr)
    {
        remitsPayPeriods << QString::fromStdString(v.payPeriodDate);
        remitTypes << QString::fromStdString(RemittanceType_to_string(v.contrib_Type));
    }
    remitTypes.removeDuplicates();
    remitsPayPeriods.removeDuplicates();
    ui->contributionTypeFilter->addItems(remitTypes);
    ui->payPeriodFilter->addItems(remitsPayPeriods);
    model = new GovernmentRemittanceModel(this, remitsPtr);
    m_proxy = new GovernmentRemittanceFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->remitTableView->setModel(m_proxy);
    ui->remitTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->remitTableView->setSortingEnabled(true);

    connect(ui->employeeSearchFilter, &QLineEdit::textChanged, this, &GovernRemittanceWidget::onEmployeeSearchChanged);
    connect(ui->payPeriodFilter, &QComboBox::currentIndexChanged, this, &GovernRemittanceWidget::onRemittancePeriodChanged);
    connect(ui->contributionTypeFilter, &QComboBox::currentIndexChanged, this, &GovernRemittanceWidget::onContributionChanged);
    connect(ui->resetButton, &QPushButton::clicked, this, &GovernRemittanceWidget::onResetClicked);
    connect(ui->exportCSVButton, &QPushButton::clicked, this, &GovernRemittanceWidget::onExportCSVClicked);
}
GovernRemittanceWidget::~GovernRemittanceWidget() { delete ui; }

void GovernRemittanceWidget::onEmployeeSearchChanged(const QString &filter)
{
    m_proxy->setEmployeeFilter(filter);
}
void GovernRemittanceWidget::onRemittancePeriodChanged()
{
    QString selected = ui->payPeriodFilter->currentText();
    LOG_DEBUG(selected.toStdString());
    m_proxy->setPayPeriodFilter(selected);
}
void GovernRemittanceWidget::onContributionChanged()
{
    QString contribution = ui->contributionTypeFilter->currentText();
    LOG_DEBUG(contribution.toStdString());
    m_proxy->setcontributionTypeFilter(contribution);
}
void GovernRemittanceWidget::onResetClicked()
{
    ui->employeeSearchFilter->clear();

    ui->payPeriodFilter->blockSignals(true);
    ui->payPeriodFilter->setCurrentIndex(-1);
    ui->payPeriodFilter->blockSignals(false);

    ui->contributionTypeFilter->blockSignals(true);
    ui->contributionTypeFilter->setCurrentIndex(-1);
    ui->contributionTypeFilter->blockSignals(false);

    m_proxy->setEmployeeFilter("");
    m_proxy->setPayPeriodFilter("");
    m_proxy->setcontributionTypeFilter("");
}
void GovernRemittanceWidget::onExportCSVClicked()
{
    CsvExporter::exportModel(this->m_proxy, this);
}
