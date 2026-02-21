#include "include/Generated/ui_payroll_registers.h"
#include "UI/Reports-Logs/Payroll_Register_Widget.h"
PayrollRegisterWidget::PayrollRegisterWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PayrollRegisterWidget), payrollByMonth{}, payrollByfortnight{}, departments{}
{
    ui->setupUi(this);
    payrollVecMonthly = AppContext::instance().payrollService().getAllPayrollsAggregatedToMonthly();
    payrollFortNight = AppContext::instance().payrollService().getAllPayrolls();
    activePayroll = &payrollVecMonthly; // default
    for (const auto &v : *activePayroll)
    {
        payrollByMonth << QString::fromStdString(v.payPeriodText);
        payrollByfortnight << QString::fromStdString(v.payPeriodText);
        departments << QString::fromStdString(v.employeeDepartment);
    }

    payrollByMonth.removeDuplicates();
    payrollByfortnight.removeDuplicates();

    departments.removeDuplicates();
    ui->departmentFilter->addItems(departments);
    connect(ui->monthlyRadio, &QRadioButton::toggled, this, &PayrollRegisterWidget::onGranularityChanged);
    connect(ui->fortNightRadio, &QRadioButton::toggled, this, &PayrollRegisterWidget::onGranularityChanged);
    connect(ui->employeeSearchFilter, &QLineEdit::textChanged, this, &PayrollRegisterWidget::onEmployeeSearchChanged);
    connect(ui->payrollPeriodFilter, &QComboBox::currentIndexChanged, this, &PayrollRegisterWidget::onPayrollPeriodChanged);
    connect(ui->departmentFilter, &QComboBox::currentIndexChanged, this, &PayrollRegisterWidget::onDepartmentChanged);
    connect(ui->resetButton, &QPushButton::clicked, this, &PayrollRegisterWidget::onResetClicked);

    model = new PayrollRegisterModel(this, activePayroll);
    // load the combo box options
    populatePayPeriodDropDownByMonth(); // default
    m_proxy = new PayrollFilterProxyModel(this);
    m_proxy->setSourceModel(model);
    m_proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->payrollTableView->setModel(m_proxy);
    ui->payrollTableView->hideColumn(4);
    ui->payrollTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->payrollTableView->setSortingEnabled(true);
}

void PayrollRegisterWidget::onPayrollPeriodChanged()
{
    QString selected = ui->payrollPeriodFilter->currentText();
    if (ui->fortNightRadio->isChecked())
    {
        // selected text for example could be 'March 2025 First Half'
        QStringList parts = selected.split(' ');
        QString half = parts[2] + " " + parts[3];
        QString monthYear = parts[0] + " " + parts[1];
        m_proxy->setPayPeriodFilter(monthYear); // Pass in 'March 2025'
        m_proxy->setPayPeriodHalfFilter(half);  // Pass in 'First Half'
    }
    else
    {
        m_proxy->setPayPeriodFilter(selected); // combo box only has the first part 'March 2025'. pass in as normal
        m_proxy->setPayPeriodHalfFilter("");   // make sure to clear the internal variable for the period half
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
void PayrollRegisterWidget::onResetClicked()
{

    ui->employeeSearchFilter->clear();

    ui->payrollPeriodFilter->blockSignals(true);
    ui->payrollPeriodFilter->setCurrentIndex(-1);
    ui->payrollPeriodFilter->blockSignals(false);

    ui->departmentFilter->blockSignals(true);
    ui->departmentFilter->setCurrentIndex(-1);
    ui->departmentFilter->blockSignals(false);

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
    // updating the model's data
    activePayroll = &payrollVecMonthly;
    model->reloadData(activePayroll);
    // We dont need this column, would normally show first or second pay period of the month
    ui->payrollTableView->hideColumn(4);

    // updating the drop down menu
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
    // updating the model's data
    activePayroll = &payrollFortNight;
    model->reloadData(activePayroll);

    // We need this column, would normally show first or second pay period of the month
    ui->payrollTableView->showColumn(4);

    // updating the drop down menu
    ui->payrollPeriodFilter->clear();
    std::sort(payrollByfortnight.begin(), payrollByfortnight.end(), [](const QString &a, const QString &b)
              {
        QDate da = QDate::fromString(a, "MMMM yyyy");
        QDate db = QDate::fromString(b, "MMMM yyyy");
        return da < db; });

    // for each month, append both halves
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