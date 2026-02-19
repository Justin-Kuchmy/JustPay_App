#include "include/Generated/ui_payroll_registers.h"
#include "UI/Reports-Logs/Payroll_Register_Widget.h"
PayrollRegisterWidget::PayrollRegisterWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PayrollRegisterWidget), payrollVec{}, payrollByMonth{}, payrollByfortnight{}, departments{}
{
    ui->setupUi(this);
    payrollVec = AppContext::instance().payrollService().getAllPayrolls();

    for (const auto &v : payrollVec)
    {
        payrollByMonth << QString::fromStdString(v.payPeriodText);
        payrollByfortnight << QString::fromStdString(v.payPeriodText);
        departments << QString::fromStdString(v.employeeDepartment);
    }

    payrollByMonth.removeDuplicates();
    payrollByfortnight.removeDuplicates();
    populatePayPeriodDropDownByMonth(); // default

    departments.removeDuplicates();
    ui->departmentFilter->addItems(departments);

    connect(ui->monthlyRadio, &QRadioButton::toggled, this, &PayrollRegisterWidget::onGranularityChanged);
    connect(ui->fortNightRadio, &QRadioButton::toggled, this, &PayrollRegisterWidget::onGranularityChanged);
    connect(ui->employeeSearchFilter, &QLineEdit::textChanged, this, &PayrollRegisterWidget::onEmployeeSearchChanged);
    connect(ui->payrollPeriodFilter, &QComboBox::currentIndexChanged, this, &PayrollRegisterWidget::onPayrollPeriodChanged);
    connect(ui->departmentFilter, &QComboBox::currentIndexChanged, this, &PayrollRegisterWidget::onDepartmentChanged);
    connect(ui->applyButton, &QPushButton::clicked, this, &PayrollRegisterWidget::onApplyClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &PayrollRegisterWidget::onResetClicked);

    model = new PayrollRegisterModel(this, payrollVec);

    m_proxy = new PayrollFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    // m_proxy->setFilterKeyColumn(0);
    ui->payrollTableView->setModel(m_proxy);
    ui->payrollTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->payrollTableView->setSortingEnabled(true);
}

void PayrollRegisterWidget::onPayrollPeriodChanged()
{
    QString selected = ui->payrollPeriodFilter->currentText();
    if (ui->fortNightRadio->isChecked())
    {
        QStringList parts = selected.split(' ');
        QString half = parts[2] + " " + parts[3];
        QString monthYear = parts[0] + " " + parts[1];
        m_proxy->setPayPeriodFilter(monthYear);
        m_proxy->setPayPeriodHalfFilter(half);
    }
    else
    {
        m_proxy->setPayPeriodFilter(selected);
    }
}

void PayrollRegisterWidget::onDepartmentChanged()
{
    QString department = ui->departmentFilter->currentText();
    m_proxy->setDepartmentFilter(department);
}

void PayrollRegisterWidget::onEmployeeSearchChanged(const QString &filter)
{
    m_proxy->setEmployeeFilter(filter);
}
void PayrollRegisterWidget::onApplyClicked()
{
    // qDebug() << "onApplyClicked";
}
void PayrollRegisterWidget::onResetClicked()
{
    ui->employeeSearchFilter->clear();
    ui->payrollPeriodFilter->setCurrentIndex(0);
    ui->departmentFilter->setCurrentIndex(0);

    m_proxy->setEmployeeFilter("");
    m_proxy->setPayPeriodFilter("");
    m_proxy->setDepartmentFilter("");
}

void PayrollRegisterWidget::onGranularityChanged()
{
    if (ui->monthlyRadio->isChecked())
    {
        populatePayPeriodDropDownByMonth();
    }
    else
    {
        populatePayPeriodDropDownByFortNight();
    }
}

void PayrollRegisterWidget::populatePayPeriodDropDownByMonth()
{
    ui->payrollPeriodFilter->blockSignals(true);
    ui->payrollPeriodFilter->clear();
    std::sort(payrollByMonth.begin(), payrollByMonth.end(), [](const QString &a, const QString &b)
              {
            QDate da = QDate::fromString(a, "MMMM yyyy");
            QDate db = QDate::fromString(b, "MMMM yyyy");
            return da < db; });
    ui->payrollPeriodFilter->addItems(payrollByMonth);
    ui->payrollPeriodFilter->blockSignals(false);
    ui->payrollPeriodFilter->setCurrentIndex(-1);
}

void PayrollRegisterWidget::populatePayPeriodDropDownByFortNight()
{
    ui->payrollPeriodFilter->blockSignals(true);
    ui->payrollPeriodFilter->clear();
    std::sort(payrollByfortnight.begin(), payrollByfortnight.end(), [](const QString &a, const QString &b)
              {
        QDate da = QDate::fromString(a, "MMMM yyyy");
        QDate db = QDate::fromString(b, "MMMM yyyy");
        return da < db; });

    // For each month, append both halves
    for (const QString &month : payrollByfortnight)
    {
        ui->payrollPeriodFilter->addItem(month + " First Half");
        ui->payrollPeriodFilter->addItem(month + " Second Half");
    }
    ui->payrollPeriodFilter->blockSignals(false);
    ui->payrollPeriodFilter->setCurrentIndex(-1);
}

PayrollRegisterWidget::~PayrollRegisterWidget()
{
    delete ui;
}