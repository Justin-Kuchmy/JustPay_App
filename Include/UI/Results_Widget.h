#ifndef RESULTSWIDGET_H
#define RESULTSWIDGET_H
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/BaseContentWidget.h"

namespace Ui{
class ResultsWidget;
}

class ResultsWidget : public BaseContentWidget {
    Q_OBJECT

public:
    explicit ResultsWidget(std::vector<PayrollCalculationResults>* dataBus, QWidget *parent = nullptr);
    ~ResultsWidget();   
    ResultsWidget(const ResultsWidget&) = delete; 
    ResultsWidget& operator=(const ResultsWidget&) = delete; 
    void calculateValues();            

private slots:

private:
    Ui::ResultsWidget *ui;
    std::vector<PayrollCalculationResults>* dataBus = nullptr;
};

#endif