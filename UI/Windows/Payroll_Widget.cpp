#include "include/Generated/ui_payroll_widget.h"
#include "UI/Payroll_Widget.h"

PayrollWidget::PayrollWidget(QWidget *parent): BaseContentWidget(parent), ui(new Ui::PayrollWidget)
{
    ui->setupUi(this);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
}
PayrollWidget::~PayrollWidget()
{

}
