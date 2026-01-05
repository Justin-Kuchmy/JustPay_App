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
    connect(premiumValue, &PremiumValuesWidget::valuesApplied, this, [this]() {
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
    
    if (pageNumber <= stack->count() - 1) {
        pageNumber = stack->currentIndex()+1;
        stack->setCurrentIndex(pageNumber);
    }
    switch(pageNumber)
    {
        case 1:
            ui->nextButton->setEnabled(false);
            break;
        case 2: 
            results->loadTableData();
            break;
        default:
            break;
    }
}

void PayrollWidget::previousWindow()
{
    QStackedWidget* stack = ui->stackedWidget;
    
    if (pageNumber >= 0) {
        pageNumber = stack->currentIndex()-1;
        stack->setCurrentIndex(pageNumber);
    }
    switch(pageNumber)
    {
        case 0:
            ui->nextButton->setEnabled(false);
            LOG_DEBUG("Back: employeeValidation Page: " << stack->currentIndex());
            break;
        case 1:

            LOG_DEBUG("Back: Premium Values Page: " << stack->currentIndex());
            break;
        default:
            break;
    }
}



