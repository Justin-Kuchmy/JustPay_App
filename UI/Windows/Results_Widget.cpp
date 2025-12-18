#include "include/Generated/ui_results_widget.h"
#include "UI/Results_Widget.h"

ResultsWidget::ResultsWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent): BaseContentWidget(parent), ui(new Ui::ResultsWidget), dataBus(dataBus)
{
    ui->setupUi(this);
}
ResultsWidget::~ResultsWidget()
{
    delete ui;
    delete dataBus;
}