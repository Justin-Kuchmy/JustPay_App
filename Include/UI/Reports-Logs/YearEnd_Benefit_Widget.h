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
    YearEndBenefitWidget(const YearEndBenefitWidget &) = delete;
    YearEndBenefitWidget &operator=(const YearEndBenefitWidget &) = delete;

public slots:
    void onYearChanged();
    void onEmployeeSearchChanged(const QString &filter);
    void onResetClicked();
    void onExportClicked();

private:
    Ui::YearEndBenefitWidget *ui;
    QStringList yearStringList;

    std::vector<YearEndBenefits> yearEndBenefits;
    std::vector<YearEndBenefits> *yearEndBenefitsPtr = nullptr;
    YearEndBenefitsModel *model;
    YearEndBenefitsFilterProxyModel *m_proxy;
};

#endif