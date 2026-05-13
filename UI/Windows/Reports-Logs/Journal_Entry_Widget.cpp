#include "include/Generated/ui_journal_entries.h"
#include "UI/Reports-Logs/Journal_Entry_Widget.h"
#include "Helpers/CsvExporter.h"
JournalEntryWidget::JournalEntryWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::JournalEntryWidget)
{
    ui->setupUi(this);
    entries = AppContext::instance().journalEntryService().getAll();
    entriesPtr = &entries;

    for (auto &v : *entriesPtr)
    {
        entryPayPeriods.append(QString::fromStdString(v.periodHalf + " " + v.periodText));
        debitTotals += v.debit;
        creditTotals += v.credit;
    }
    entryPayPeriods.removeDuplicates();
    ui->payPeriodFilter->addItems(entryPayPeriods);

    model = new JournalEntryModel(this, &entries);
    m_proxy = new JournalEntryFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->journalEntriesTableView->setModel(m_proxy);
    ui->journalEntriesTableView->hideColumn(4);
    ui->journalEntriesTableView->hideColumn(5);
    ui->journalEntriesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->journalEntriesTableView->setSortingEnabled(true);

    ui->payPeriodFilter->setCurrentIndex(-1);

    ui->debitTotalLabel->setText(QString("₱%1").arg(phLocale.toString(debitTotals, 'f', 2)));
    ui->creditTotalLabel->setText(QString("₱%1").arg(phLocale.toString(creditTotals, 'f', 2)));

    connect(ui->payPeriodFilter, &QComboBox::currentIndexChanged, this, &JournalEntryWidget::onPayrollPeriodChanged);
    connect(ui->exportCSVButton, &QPushButton::clicked, this, &JournalEntryWidget::onExportCSVClicked);
}

void JournalEntryWidget::onPayrollPeriodChanged()
{
    debitTotals = 0;
    creditTotals = 0;
    QString selected = ui->payPeriodFilter->currentText();
    LOG_DEBUG("selected: [" << selected.toStdString() << "]");
    QStringList parts = selected.split(' ');
    QString half = parts[2] + " " + parts[3];
    QString monthYear = parts[0] + " " + parts[1];
    m_proxy->setPayPeriodFilter(monthYear);
    m_proxy->setPayPeriodHalfFilter(half);
    for (int row = 0; row < m_proxy->rowCount(); ++row)
    {
        // int rows = m_proxy->rowCount();
        QModelIndex proxyIndex = m_proxy->index(row, 3);
        QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);

        const JournalEntry &entry = model->getFiltered_Model()->at(sourceIndex.row());
        debitTotals += entry.debit;
        creditTotals += entry.credit;
    }
    ui->debitTotalLabel->setText(QString("₱%1").arg(phLocale.toString(debitTotals, 'f', 2)));
    ui->creditTotalLabel->setText(QString("₱%1").arg(phLocale.toString(creditTotals, 'f', 2)));
}

void JournalEntryWidget::onExportCSVClicked()
{
    CsvExporter::exportModel(this->m_proxy, this);
}

JournalEntryWidget::~JournalEntryWidget() { delete ui; }
