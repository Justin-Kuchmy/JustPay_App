#include "include/Generated/ui_payroll_widget.h"
#include "UI/Payroll_Widget.h"

PayrollWidget::PayrollWidget(QWidget *parent): BaseContentWidget(parent), ui(new Ui::PayrollWidget)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    connect(ui->previousButton, &QPushButton::clicked, this, &PayrollWidget::previousWindow);
    connect(ui->nextButton, &QPushButton::clicked, this, &PayrollWidget::nextWindow);
}
PayrollWidget::~PayrollWidget()
{
    delete ui;
}

void PayrollWidget::nextWindow()
{
    QStackedWidget* stack = ui->stackedWidget;
    int index = stack->currentIndex();

    if (index <= stack->count() - 1) {
        stack->setCurrentIndex(index + 1);
    }
    LOG_DEBUG("now viewing page: "<<  index);
}
void PayrollWidget::previousWindow()
{
    QStackedWidget* stack = ui->stackedWidget;
    int index = stack->currentIndex();

    if (index >= 0) {
        stack->setCurrentIndex(index - 1);
    }
    LOG_DEBUG("now viewing page: "<<  index);
}



