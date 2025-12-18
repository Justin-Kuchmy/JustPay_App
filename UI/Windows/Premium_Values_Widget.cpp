#include "include/Generated/ui_premium_values_widget.h"
#include "UI/Premium_Values_Widget.h"

PremiumValuesWidget::PremiumValuesWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::PremiumValuesWidget), dataBus(dataBus)
{
    ui->setupUi(this);
}
PremiumValuesWidget::~PremiumValuesWidget()
{
    delete ui;
    delete dataBus;
}