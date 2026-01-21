#ifndef PAYSLIPWIDGET_H
#define PAYSLIPWIDGET_H

#include <QWidget>
#include <QTextDocument>
#include <QPrinter>
#include "Utils/BaseContentWidget.h"
#include "Models/DataObjects.h"
#include <QDir>

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
    QString generatePDF(const PayrollCalculationResults &payslip, const QString &outputDir = QString());
    void sendEmailWithAttachment(const QString &recipientEmail, const QString &attachmentPath, const QString &subject = "Payslip", const QString &body = "Please find your payslip attached.");
    void printPayslip(const PayrollCalculationResults &payslip);
    void printPayslip(const QString &pdfPath);

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