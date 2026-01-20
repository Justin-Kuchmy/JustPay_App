#include "include/Generated/ui_payslip_widget.h"
#include "UI/Payslip_Generation_Widget.h"
#include "Utils/Log.h"

PayslipWidget::PayslipWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PayslipWidget)
{
    ui->setupUi(this);
    employees = AppContext::instance().employeeService().getAllEmployees();
    ui->comboPayrollPeriod->addItems({
        QString::fromStdString("First"),
        QString::fromStdString("Last"),
    });
    for (const Employee &e : employees)
    {
        ui->employeeComboBox->addItem(QString::fromStdString(e.fullName), QVariant::fromValue(e.employeeId));
    }
    connect(ui->generateAllButton, &QPushButton::clicked, this, &PayslipWidget::onGenerateAllClicked);
    connect(ui->generateSelectedButton, &QPushButton::clicked, this, &PayslipWidget::onGenerateOneClicked);
    connect(ui->printButton, &QPushButton::clicked, this, &PayslipWidget::onPrintClicked);
    connect(ui->emailButton, &QPushButton::clicked, this, &PayslipWidget::onEmailClicked);
}

PayslipWidget::~PayslipWidget()
{
    delete ui;
}

void PayslipWidget::onGenerateAllClicked()
{
    qDebug("PayslipWidget::onGenerateAllClicked()");
}

void PayslipWidget::onGenerateOneClicked()
{
    qDebug("PayslipWidget::onGenerateOneClicked()");
}

void PayslipWidget::onPrintClicked()
{
    qDebug("PayslipWidget::onPrintClicked()");
}

void PayslipWidget::onEmailClicked()
{
    qDebug("PayslipWidget::onEmailClicked()");
}
