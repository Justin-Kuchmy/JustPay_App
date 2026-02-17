#include "include/Generated/ui_gov_remittance.h"
#include "UI/Govern_Remittance_Widget.h"
GovernRemittanceWidget::GovernRemittanceWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::GovernRemittanceWidget)
{
    ui->setupUi(this);
}
GovernRemittanceWidget::~GovernRemittanceWidget() { delete ui; }
