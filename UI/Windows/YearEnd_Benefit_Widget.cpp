#include "include/Generated/ui_year-end_benefits.h"
#include "UI/YearEnd_Benefit_Widget.h"
YearEndBenefitWidget::YearEndBenefitWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::YearEndBenefitWidget)
{
    ui->setupUi(this);
}
YearEndBenefitWidget::~YearEndBenefitWidget()
{
    delete ui;
}
