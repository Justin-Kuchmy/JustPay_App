#include "include/Generated/ui_results_widget.h"
#include "UI/Results_Widget.h"

enum TableColumn {
    COL_EMPLOYEE_ID = 0,
    COL_FULL_NAME,
    COL_DEPARTMENT,
    COL_BASIC_SALARY,
    COL_ALLOWANCES,
    COL_OVERTIME,
    COL_GROSS,
    COL_SSS,
    COL_PHILHEALTH,
    COL_HDMF,
    COL_LOAN,
    COL_DED_1ST,
    COL_DED_2ND,
    COL_TAX,
    COL_NET_PAY,

    COL_COUNT  
};

ResultsWidget::ResultsWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::ResultsWidget), dataBus(dataBus)
{
    ui->setupUi(this);
    model = std::make_unique<QStandardItemModel>(this);
    
}
ResultsWidget::~ResultsWidget()
{
    delete ui;
}

void ResultsWidget::loadTableData()
{
    ui->table_results->setRowCount(dataBus->size());
    ui->table_results->setColumnCount(COL_COUNT);
    ui->value_payPeriod->setText(QString::fromStdString(dataBus->at(0).payrollPeriod));
    ui->value_dateProcessed->setText(QString::fromStdString(dataBus->at(0).dateProcessed.to_string()));
    for (size_t i{}; i < dataBus->size(); i++)
    {
        QTableWidgetItem *EMPLOYEE_ID = new QTableWidgetItem(QString::fromStdString(dataBus->at(i).employeeId));
        QTableWidgetItem *FULL_NAME = new QTableWidgetItem(QString::fromStdString(dataBus->at(i).fullName));
        QTableWidgetItem *DEPARTMENT = new QTableWidgetItem(QString::fromStdString(dataBus->at(i).employeeDepartment));
        QTableWidgetItem *BASIC_SALARY = new QTableWidgetItem(QString::number(dataBus->at(i).monthlyBasicSalary, 'f', 2));
        QTableWidgetItem *ALLOWANCES = new QTableWidgetItem(QString::number(dataBus->at(i).monthlyAllowances, 'f', 2));
        QTableWidgetItem *OVERTIME = new QTableWidgetItem(QString::number(dataBus->at(i).overTimePay, 'f', 2));
        QTableWidgetItem *GROSS = new QTableWidgetItem(QString::number(dataBus->at(i).adjustments, 'f', 2));
        QTableWidgetItem *SSS = new QTableWidgetItem(QString::number(dataBus->at(i).grossIncome, 'f', 2));
        QTableWidgetItem *PHILHEALTH = new QTableWidgetItem(QString::number(dataBus->at(i).sssPremium, 'f', 2));
        QTableWidgetItem *HDMF = new QTableWidgetItem(QString::number(dataBus->at(i).philHealthPremium, 'f', 2));
        QTableWidgetItem *LOAN = new QTableWidgetItem(QString::number(dataBus->at(i).hdmfPremium, 'f', 2));
        QTableWidgetItem *DED_1ST = new QTableWidgetItem(QString::number(dataBus->at(i).loanDeductionsPerPayroll, 'f', 2));
        QTableWidgetItem *DED_2ND = new QTableWidgetItem(QString::number(dataBus->at(i).withHoldingTax, 'f', 2));
        QTableWidgetItem *TAX = new QTableWidgetItem(QString::number(dataBus->at(i).totalDeductions, 'f', 2));
        QTableWidgetItem *NET_PAY = new QTableWidgetItem(QString::number(dataBus->at(i).netPay, 'f', 2));

        ui->table_results->setItem(i, COL_EMPLOYEE_ID, EMPLOYEE_ID);
        ui->table_results->setItem(i, COL_FULL_NAME, FULL_NAME);
        ui->table_results->setItem(i, COL_DEPARTMENT, DEPARTMENT);
        ui->table_results->setItem(i, COL_BASIC_SALARY, BASIC_SALARY);
        ui->table_results->setItem(i, COL_ALLOWANCES, ALLOWANCES);
        ui->table_results->setItem(i, COL_OVERTIME, OVERTIME);
        ui->table_results->setItem(i, COL_GROSS, GROSS);
        ui->table_results->setItem(i, COL_SSS, SSS);
        ui->table_results->setItem(i, COL_PHILHEALTH, PHILHEALTH);
        ui->table_results->setItem(i, COL_HDMF, HDMF);
        ui->table_results->setItem(i, COL_LOAN, LOAN);
        ui->table_results->setItem(i, COL_DED_1ST, DED_1ST);
        ui->table_results->setItem(i, COL_DED_2ND, DED_2ND);
        ui->table_results->setItem(i, COL_TAX, TAX);
        ui->table_results->setItem(i, COL_NET_PAY, NET_PAY);

    }
}