#include "include/Generated/ui_year-end_benefits.h"
#include "UI/Reports-Logs/YearEnd_Benefit_Widget.h"
YearEndBenefitWidget::YearEndBenefitWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::YearEndBenefitWidget)
{
    ui->setupUi(this);
}
YearEndBenefitWidget::~YearEndBenefitWidget()
{
    delete ui;
}
