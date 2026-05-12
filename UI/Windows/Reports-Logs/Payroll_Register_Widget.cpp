#include "include/Generated/ui_payroll_registers.h"
#include "UI/Reports-Logs/Payroll_Register_Widget.h"
PayrollRegisterWidget::PayrollRegisterWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PayrollRegisterWidget), payrollByMonth{}, payrollByfortnight{}, departments{}
{
    ui->setupUi(this);
    LOG_DEBUG("Loading PayrollRegisterWidget");
    payrollVecMonthly = AppContext::instance().payrollService().getAllPayrollsAggregatedToMonthly();
    payrollFortNight = AppContext::instance().payrollService().getAllPayrolls();
    activePayroll = &payrollVecMonthly; // default
    for (const auto &v : *activePayroll)
    {
        payrollByMonth << QString::fromStdString(v.payPeriodDate);
        payrollByfortnight << QString::fromStdString(v.payPeriodDate);
        departments << QString::fromStdString(department_to_string(v.employeeDepartment));
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
    connect(ui->exportCSVButton, &QPushButton::clicked, this, &PayrollRegisterWidget::exportCSVClicked);

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
    LOG_DEBUG("finished Loading PayrollRegisterWidget");
}

void PayrollRegisterWidget::exportCSVClicked()
{
    if (!this->m_proxy)
        return;
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Save CSV",
        "",
        "CSV files (*.csv);;All files (*)");
    if (fileName.isEmpty())
        return;
    if (!fileName.endsWith(".csv", Qt::CaseInsensitive))
    {
        fileName += ".csv";
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open file for writing");
        return;
    }

    QTextStream out(&file);
    int columns = this->m_proxy->columnCount();
    int rows = this->m_proxy->rowCount();

    for (int j = 0; j < columns; ++j)
    {
        QString header = this->m_proxy->headerData(j, Qt::Horizontal).toString();
        if (!ui->payrollTableView->isColumnHidden(j))
        {
            out << "\"" << header << "\"";
            if (j < columns - 1)
                out << ",";
        }
    }
    out << "\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QModelIndex index = m_proxy->index(i, j);
            QString data = m_proxy->data(index, Qt::DisplayRole).toString();
            if (!ui->payrollTableView->isColumnHidden(j))
            {
                out << "\"" << data << "\"";
                if (j < columns - 1)
                    out << ",";
            }
        }
        out << "\n";
    }
    file.close();
    QMessageBox::information(this, "Export CSV", "CSV file saved successfully!");
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
        m_proxy->setPayPeriodHalfFilter("");
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