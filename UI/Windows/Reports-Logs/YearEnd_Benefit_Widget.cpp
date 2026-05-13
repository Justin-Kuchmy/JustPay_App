#include "include/Generated/ui_year-end_benefits.h"
#include "UI/Reports-Logs/YearEnd_Benefit_Widget.h"
YearEndBenefitWidget::YearEndBenefitWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::YearEndBenefitWidget)
{
    ui->setupUi(this);
    yearEndBenefits = AppContext::instance().yearEndBenefitsService().compute();
    yearEndBenefitsPtr = &yearEndBenefits;
    for (const auto &y : *yearEndBenefitsPtr)
    {
        yearStringList << QString::fromStdString(std::to_string(y.year));
    }
    yearStringList.removeDuplicates();
    ui->yearComboBox->addItems(yearStringList);
    model = new YearEndBenefitsModel(this, yearEndBenefitsPtr);
    m_proxy = new YearEndBenefitsFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->yearEndTableView->setModel(m_proxy);
    ui->yearEndTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->yearEndTableView->setSortingEnabled(true);

    connect(ui->yearComboBox, &QComboBox::currentIndexChanged, this, &YearEndBenefitWidget::onYearChanged);
    connect(ui->employeeSearchFilter, &QLineEdit::textChanged, this, &YearEndBenefitWidget::onEmployeeSearchChanged);
    connect(ui->resetButton, &QPushButton::clicked, this, &YearEndBenefitWidget::onResetClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &YearEndBenefitWidget::onExportClicked);
    // employeeSearchFilter
}
YearEndBenefitWidget::~YearEndBenefitWidget()
{
    delete ui;
}

void YearEndBenefitWidget::onYearChanged()
{
    QString year = ui->yearComboBox->currentText();
    m_proxy->setYearFilter(year);
}
void YearEndBenefitWidget::onEmployeeSearchChanged(const QString &filter)
{
    m_proxy->setEmployeeFilter(filter);
}
void YearEndBenefitWidget::onResetClicked()
{
    ui->employeeSearchFilter->clear();

    ui->yearComboBox->blockSignals(true);
    ui->yearComboBox->setCurrentIndex(-1);
    ui->yearComboBox->blockSignals(false);

    m_proxy->setEmployeeFilter("");
    m_proxy->setYearFilter("");
}
void YearEndBenefitWidget::onExportClicked()
{
}
