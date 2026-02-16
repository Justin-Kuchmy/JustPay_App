#ifndef PAYSLIPWIDGET_H
#define PAYSLIPWIDGET_H

#include <QWidget>
#include <QTextDocument>
#include <QMessageBox>
#include <QPrintDialog>
#include <QPrinter>
#include <QSslSocket>
#include "Utils/BaseContentWidget.h"
#include "Models/DataObjects.h"
#include <QDir>
#include <fstream>
#include <QFileDialog>

namespace Ui
{
    class PayslipWidget;
}

enum class ActionType
{
    PrintOrSave,
    Email
};

class PayslipWidget : public BaseContentWidget
{
    Q_OBJECT
public:
    explicit PayslipWidget(QWidget *parent = nullptr);
    ~PayslipWidget();
    PayslipWidget(const PayslipWidget &) = delete;
    PayslipWidget &operator=(const PayslipWidget &) = delete;
    QString htmlFileToQString(const PayrollCalculationResults &payslip);
    PayrollCalculationResults getPayslipForEmployeeAndPeriod(QString &employeeId);
    QString payrollPeriod(const QString &monthYear, bool firstHalf);

    QString generatePdfFromHtmlInMemory(QString &html);
    void handleEmployeeOrUpload(ActionType action);
    void sendEmail(const QString &pdfFilePath);
    QString getSuggestedFileName(PayrollCalculationResults &payslip);
    QString showOptionDialog(QString title, QString text, QString opt1, QString opt2);

private slots:
    void onGenerateAllClicked();
    void onGenerateOneOrPrint();
    void onEmailClicked();
    void onPreviewClicked();
    void onComboBoxChanged();

signals:

private:
    Ui::PayslipWidget *ui;
    std::vector<Employee> employees{};
    Employee selectedEmployee{};
    QString selectedEmployeeId{};
    QString payPeriod{};
    QTextDocument pdfDoc{};
};

#endif