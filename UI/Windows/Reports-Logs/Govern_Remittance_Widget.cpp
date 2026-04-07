#include "include/Generated/ui_gov_remittance.h"
#include "UI/Reports-Logs/Govern_Remittance_Widget.h"
GovernRemittanceWidget::GovernRemittanceWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::GovernRemittanceWidget)
{
    ui->setupUi(this);
    remits = AppContext::instance().governmentRemittanceService().getAll();
    remitsPtr = &remits;

    for (auto &v : *remitsPtr)
    {
        remitsPayPeriods.append(QString::fromStdString(v.payPeriodText));
        remitTypes.append(QString::fromStdString(RemittanceType_to_string(v.contrib_Type)));
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
}
GovernRemittanceWidget::~GovernRemittanceWidget() { delete ui; }
