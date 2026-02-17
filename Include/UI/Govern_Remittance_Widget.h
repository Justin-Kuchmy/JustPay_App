#ifndef GOVERNREMITTANCEWIDGET_H
#define GOVERNREMITTANCEWIDGET_H
#include <QWidget>
#include "Utils/BaseContentWidget.h"
namespace Ui
{
    class GovernRemittanceWidget;
}
class GovernRemittanceWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    GovernRemittanceWidget(QWidget *parent = nullptr);
    ~GovernRemittanceWidget();

private:
    Ui::GovernRemittanceWidget *ui;
};
#endif