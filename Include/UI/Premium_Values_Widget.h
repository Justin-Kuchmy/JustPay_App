#ifndef PREMIUMVALUESWIDGET_H
#define PREMIUMVALUESWIDGET_H
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"


namespace Ui{
class PremiumValuesWidget;
}

class PremiumValuesWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit PremiumValuesWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent = nullptr);
    ~PremiumValuesWidget();
    PremiumValuesWidget(const PremiumValuesWidget&) = delete; 
    PremiumValuesWidget& operator=(const PremiumValuesWidget&) = delete; 
    void applyValues();
    
private slots:
    void applyClicked();

private:
    Ui::PremiumValuesWidget *ui;
    std::vector<PayrollCalculationResults>* dataBus = nullptr;
};

#endif