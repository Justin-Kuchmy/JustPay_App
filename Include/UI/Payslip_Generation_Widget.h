#ifndef PAYSLIPWIDGET_H
#define PAYSLIPWIDGET_H

#include <QWidget>
#include "Utils/BaseContentWidget.h"
#include "Models/DataObjects.h"

namespace Ui
{
    class PayslipWidget;
}

class PayslipWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    explicit PayslipWidget(QWidget *parent = nullptr);
    ~PayslipWidget();
    PayslipWidget(const PayslipWidget &) = delete;
    PayslipWidget &operator=(const PayslipWidget &) = delete;

private slots:
    void onGenerateAllClicked();
    void onGenerateOneClicked();
    void onPrintClicked();
    void onEmailClicked();

signals:

private:
    Ui::PayslipWidget *ui;
    std::vector<Employee> employees{};
    Employee selectedEmployee{};
    QString payPeriod{};
};

#endif