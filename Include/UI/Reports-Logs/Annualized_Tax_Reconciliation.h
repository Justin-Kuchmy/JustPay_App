#ifndef ANNUALTAXWIDGET_H
#define ANNUALTAXWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
namespace Ui
{
    class AnnualTaxWidget;
}
class AnnualTaxWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    AnnualTaxWidget(QWidget *parent = nullptr);
    ~AnnualTaxWidget();

private:
    Ui::AnnualTaxWidget *ui;
};
#endif
