#include "include/Generated/ui_payroll_registers.h"
#include "UI/Reports-Logs/Payroll_Register_Widget.h"
PayrollRegisterWidget::PayrollRegisterWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PayrollRegisterWidget)
{
    ui->setupUi(this);
}
PayrollRegisterWidget::~PayrollRegisterWidget() { delete ui; }