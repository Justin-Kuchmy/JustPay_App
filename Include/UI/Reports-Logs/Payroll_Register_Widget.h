#ifndef PAYROLLREGISTERWIDGET_H
#define PAYROLLREGISTERWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
namespace Ui
{
    class PayrollRegisterWidget;
}
class PayrollRegisterWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    PayrollRegisterWidget(QWidget *parent = nullptr);
    ~PayrollRegisterWidget();

private:
    Ui::PayrollRegisterWidget *ui;
};
#endif