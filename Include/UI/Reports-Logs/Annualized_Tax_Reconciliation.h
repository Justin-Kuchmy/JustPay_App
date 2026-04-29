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
    AnnualTaxWidget(const AnnualTaxWidget &) = delete;
    AnnualTaxWidget &operator=(const AnnualTaxWidget &) = delete;
    void onYearChanged();

private:
    Ui::AnnualTaxWidget *ui;
    QStringList yearStringList;
    QStringList departmentStringList;
    std::vector<TaxReconciliationReport> taxReports;
    std::vector<TaxReconciliationReport> *taxReportsPtr = nullptr;

    TaxReconciliationReportModel *model;
    TaxReconciliationReportFilterProxyModel *m_proxy;
};
#endif
