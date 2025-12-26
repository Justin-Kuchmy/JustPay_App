#include "include/Generated/ui_payroll_widget.h"
#include "UI/Payroll_Widget.h"


PayrollWidget::PayrollWidget(QWidget *parent): BaseContentWidget(parent), ui(new Ui::PayrollWidget),
payrollResults{}, employeeValidation{}, premiumValue{}, results{}
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    connect(ui->previousButton, &QPushButton::clicked, this, &PayrollWidget::previousWindow);
    connect(ui->nextButton, &QPushButton::clicked, this, &PayrollWidget::nextWindow);
    ui->nextButton->setEnabled(false);
    this->payrollResults_ptr = &this->payrollResults;
    employeeValidation = new EmployeeValidationWidget(payrollResults_ptr);
    premiumValue = new PremiumValuesWidget(payrollResults_ptr);
    results = new ResultsWidget(payrollResults_ptr);
    ui->stackedWidget->addWidget(employeeValidation);
    ui->stackedWidget->addWidget(premiumValue);
    ui->stackedWidget->addWidget(results);
    connect(employeeValidation, &EmployeeValidationWidget::dataLoaded, this, [this]() {
        ui->nextButton->setEnabled(true);
    });
    

}
PayrollWidget::~PayrollWidget()
{
    delete ui;
    delete employeeValidation;
    delete premiumValue;
    delete results;

}

void PayrollWidget::nextWindow()
{
    QStackedWidget* stack = ui->stackedWidget;
    int index = stack->currentIndex();

    if (index <= stack->count() - 1) {
        stack->setCurrentIndex(index + 1);
    }
}
void PayrollWidget::previousWindow()
{
    QStackedWidget* stack = ui->stackedWidget;
    int index = stack->currentIndex();

    if (index >= 0) {
        stack->setCurrentIndex(index - 1);
    }
}



