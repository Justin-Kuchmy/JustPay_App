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
    GovernRemittanceWidget(const GovernRemittanceWidget &) = delete;
    GovernRemittanceWidget &operator=(const GovernRemittanceWidget &) = delete;

public slots:
    void onEmployeeSearchChanged(const QString &filter);
    void onRemittancePeriodChanged();
    void onContributionChanged();
    void onResetClicked();
    void onExportCSVClicked();

private:
    Ui::GovernRemittanceWidget *ui;
    QStringList remitsPayPeriods;
    QStringList remitTypes;
    std::vector<GovernmentRemittance> remits;
    std::vector<GovernmentRemittance> *remitsPtr = nullptr;

    GovernmentRemittanceModel *model;
    GovernmentRemittanceFilterProxyModel *m_proxy;
    QLocale phLocale{QLocale::English, QLocale::Philippines};
};
#endif