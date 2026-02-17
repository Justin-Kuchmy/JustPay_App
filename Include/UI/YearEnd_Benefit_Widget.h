#ifndef YEARENDBENEFITWIDGET_H
#define YEARENDBENEFITWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
namespace Ui
{
    class YearEndBenefitWidget;
}
class YearEndBenefitWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    YearEndBenefitWidget(QWidget *parent = nullptr);
    ~YearEndBenefitWidget();

private:
    Ui::YearEndBenefitWidget *ui;
};

#endif