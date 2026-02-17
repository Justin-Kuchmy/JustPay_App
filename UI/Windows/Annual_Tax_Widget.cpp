#include "include/Generated/ui_annualized_tax_reconciliation.h"
#include "UI/Annualized_Tax_Reconciliation.h"

AnnualTaxWidget::AnnualTaxWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::AnnualTaxWidget)
{
    ui->setupUi(this);
}
AnnualTaxWidget::~AnnualTaxWidget() { delete ui; }
