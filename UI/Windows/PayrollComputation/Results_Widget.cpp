#include "include/Generated/ui_results_widget.h"
#include "UI/PayrollComputation/Results_Widget.h"
#include <qfiledialog.h>
#include <qmessagebox.h>

enum TableColumn
{
    COL_EMPLOYEE_ID = 0,
    COL_FULL_NAME,
    COL_DEPARTMENT,
    COL_BASIC_SALARY,
    COL_ALLOWANCES,
    COL_OVERTIME,
    COL_ADJUST,
    COL_GROSS,
    COL_SSS,
    COL_PHILHEALTH,
    COL_HDMF,
    COL_LOAN,
    COL_TAX,
    COL_DED,
    COL_NET_PAY,

    COL_COUNT
};

ResultsWidget::ResultsWidget(std::vector<PayrollCalculationResults> *dataBus, QWidget *parent) : BaseContentWidget(parent), ui(new Ui::ResultsWidget), dataBus(dataBus)
{
    ui->setupUi(this);
    model = std::make_unique<QStandardItemModel>(this);
    connect(ui->submitButton, &QPushButton::clicked, this, &ResultsWidget::submitPayroll);
    connect(ui->exportButton, &QPushButton::clicked, this, &ResultsWidget::exportToExcel);
}
ResultsWidget::~ResultsWidget()
{
    delete ui;
}

void ResultsWidget::loadTableData()
{
    ui->table_results->setRowCount(dataBus->size());
    ui->table_results->setColumnCount(COL_COUNT);
    ui->value_payPeriod->setText(QString::fromStdString(dataBus->at(0).payPeriodText));
    ui->value_dateProcessed->setText(QString::fromStdString(dataBus->at(0).dateProcessed.to_string()));
    for (size_t i{}; i < dataBus->size(); i++)
    {
        QTableWidgetItem *EMPLOYEE_ID = new QTableWidgetItem(QString::fromStdString(dataBus->at(i).employeeId));
        QTableWidgetItem *FULL_NAME = new QTableWidgetItem(QString::fromStdString(dataBus->at(i).fullName));
        QTableWidgetItem *DEPARTMENT = new QTableWidgetItem(QString::fromStdString(dataBus->at(i).employeeDepartment));
        QTableWidgetItem *BASIC_SALARY = new QTableWidgetItem(QString::number(dataBus->at(i).monthlyBasicSalary, 'f', 2));
        QTableWidgetItem *ALLOWANCES = new QTableWidgetItem(QString::number(dataBus->at(i).monthlyAllowances, 'f', 2));
        QTableWidgetItem *OVERTIME = new QTableWidgetItem(QString::number(dataBus->at(i).overTimePay, 'f', 2));
        QTableWidgetItem *ADJUST = new QTableWidgetItem(QString::number(dataBus->at(i).adjustments, 'f', 2));
        QTableWidgetItem *GROSS = new QTableWidgetItem(QString::number(dataBus->at(i).grossIncome, 'f', 2));
        QTableWidgetItem *SSS = new QTableWidgetItem(QString::number(dataBus->at(i).sssPremium, 'f', 2));
        QTableWidgetItem *PHILHEALTH = new QTableWidgetItem(QString::number(dataBus->at(i).philHealthPremium, 'f', 2));
        QTableWidgetItem *HDMF = new QTableWidgetItem(QString::number(dataBus->at(i).hdmfPremium, 'f', 2));
        QTableWidgetItem *LOAN = new QTableWidgetItem(QString::number(dataBus->at(i).loanDeductionsPerPayroll, 'f', 2));
        QTableWidgetItem *TAX = new QTableWidgetItem(QString::number(dataBus->at(i).withHoldingTax, 'f', 2));
        QTableWidgetItem *DED = new QTableWidgetItem(QString::number(dataBus->at(i).totalDeductions, 'f', 2));
        QTableWidgetItem *NET_PAY = new QTableWidgetItem(QString::number(dataBus->at(i).netPay, 'f', 2));

        ui->table_results->setItem(i, COL_EMPLOYEE_ID, EMPLOYEE_ID);
        ui->table_results->setItem(i, COL_FULL_NAME, FULL_NAME);
        ui->table_results->setItem(i, COL_DEPARTMENT, DEPARTMENT);
        ui->table_results->setItem(i, COL_BASIC_SALARY, BASIC_SALARY);
        ui->table_results->setItem(i, COL_ALLOWANCES, ALLOWANCES);
        ui->table_results->setItem(i, COL_OVERTIME, OVERTIME);
        ui->table_results->setItem(i, COL_ADJUST, ADJUST);
        ui->table_results->setItem(i, COL_GROSS, GROSS);
        ui->table_results->setItem(i, COL_SSS, SSS);
        ui->table_results->setItem(i, COL_PHILHEALTH, PHILHEALTH);
        ui->table_results->setItem(i, COL_HDMF, HDMF);
        ui->table_results->setItem(i, COL_LOAN, LOAN);
        ui->table_results->setItem(i, COL_TAX, TAX);
        ui->table_results->setItem(i, COL_DED, DED);
        ui->table_results->setItem(i, COL_NET_PAY, NET_PAY);
    }
}

void ResultsWidget::submitPayroll()
{
    std::string currentPeriod = ui->value_payPeriod->text().toStdString();
    for (size_t i{}; i < dataBus->size(); i++)
    {
        auto &results = dataBus->at(i);
        std::vector<LoanLedger> loans = AppContext::instance().loanLedgerService().getAllLoanLedgers(results.employeeId);
        for (size_t j{}; j < loans.size(); j++)
        {
            if (loans[j].status == false)
            {
                continue;
            }
            const double loanDeductionsPerPayroll = loans[j].deductionsPerPayroll;

            bool firstHalf = loans[j].deductionFirstHalf;
            bool secondHalf = loans[j].deductionSecondHalf;

            if ((firstHalf && !secondHalf && currentPeriod == "first") ||
                (secondHalf && currentPeriod == "second"))
            {
                loans[j].principalAmount -= loanDeductionsPerPayroll;
                loans[j].NumOfAmortizations--;
                if (loans[j].principalAmount <= 0)
                {
                    loans[j].principalAmount = 0;
                    loans[j].deductionFirstHalf = false;
                    loans[j].deductionSecondHalf = false;
                    loans[j].status = false;
                }
            }

            AppContext::instance().loanLedgerService().updateLoanLedger(loans[j]);
        }
    }
}

void ResultsWidget::exportToExcel()
{
    if (!model)
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
    int columns = ui->table_results->columnCount();
    int rows = ui->table_results->rowCount();

    for (int j = 0; j < columns; ++j)
    {
        QString header = ui->table_results->horizontalHeaderItem(j) ? ui->table_results->horizontalHeaderItem(j)->text() : "";
        out << "\"" << header << "\"";
        if (j < columns - 1)
            out << ",";
    }
    out << "\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            QTableWidgetItem *item = ui->table_results->item(i, j);
            QString text = item ? item->text() : "";
            out << "\"" << text << "\"";
            if (j < columns - 1)
                out << ",";
        }
        out << "\n";
    }
    file.close();
    QMessageBox::information(this, "Export CSV", "CSV file saved successfully!");
}