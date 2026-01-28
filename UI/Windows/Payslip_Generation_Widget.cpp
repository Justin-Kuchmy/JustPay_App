#include "include/Generated/ui_payslip_widget.h"
#include "UI/Payslip_Generation_Widget.h"
#include "UI/Payslip_Preview_Dialog.h"
#include "Utils/Log.h"
#include <QStandardPaths>

PayslipWidget::PayslipWidget(QWidget *parent) : BaseContentWidget(parent), ui(new Ui::PayslipWidget)
{
    ui->setupUi(this);
    employees = AppContext::instance().employeeService().getAllEmployees();
    ui->comboPayrollPeriod->addItems({
        QString::fromStdString("First"),
        QString::fromStdString("Last"),
    });
    for (const Employee &e : employees)
    {
        ui->employeeComboBox->addItem(QString::fromStdString(e.fullName), QVariant::fromValue(QString::fromStdString(e.employeeId)));
    }
    connect(ui->generateAllButton, &QPushButton::clicked, this, &PayslipWidget::onGenerateAllClicked);
    connect(ui->generateOrPrintButton, &QPushButton::clicked, this, &PayslipWidget::onGenerateOneOrPrint);
    connect(ui->emailButton, &QPushButton::clicked, this, &PayslipWidget::onEmailClicked);
    connect(ui->pdfPreviewButton, &QPushButton::clicked, this, &PayslipWidget::onPreviewClicked);
}

PayslipWidget::~PayslipWidget()
{
    delete ui;
}

PayrollCalculationResults PayslipWidget::getPayslipForEmployeeAndPeriod(QString &employeeId)
{

    auto payrollDate = ui->payrollDateEdit->date().toString("MMMM yyyy").toStdString();
    auto payrollPeriod = ui->comboPayrollPeriod->currentIndex() + 1;
    const auto payrollOpt = AppContext::instance().payrollService().getPayrollByEmployeeAndPeriod(employeeId.toStdString(), payrollDate, payrollPeriod);
    QString path{};
    if (payrollOpt.has_value())
    {
        return payrollOpt.value();
    }
    else
    {
        qDebug() << "Failed to retrieve payslip for employee ID:" << employeeId;
        return PayrollCalculationResults{};
    }
}

QString PayslipWidget::payrollPeriod(const QString &monthYear, bool firstHalf)
{

    QStringList parts = monthYear.split(' ');
    if (parts.size() != 2)
        return monthYear;

    QString monthStr = parts[0];
    int year = parts[1].toInt();

    QDate tempDate = QDate::fromString(monthStr + " " + QString::number(year), "MMMM yyyy");
    if (!tempDate.isValid())
        return monthYear;

    int month = tempDate.month();
    int daysInMonth = QDate(year, month, 1).daysInMonth();

    if (firstHalf)
        return QString("%1 1-15 %2").arg(monthStr).arg(year);
    else
        return QString("%1 16-%2 %3").arg(monthStr).arg(daysInMonth).arg(year);
}

QString PayslipWidget::htmlFileToQString(const PayrollCalculationResults &payslip)
{
    QString html;
    QFile file(":/resources/docs/payslip.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        html = file.readAll();
        file.close();

        std::string forPDF = payslip.payPeriodText;
        auto updatedPayPeriod = payrollPeriod(QString::fromStdString(forPDF), payslip.payPeriodHalf == 1).toStdString();

        html.replace("{{PayPeriod}}", updatedPayPeriod.c_str());
        html.replace("{{EmployeeID}}", payslip.employeeId.c_str());
        html.replace("{{EmployeeName}}", payslip.fullName.c_str());
        html.replace("{{BasicSalary}}", QString::number(payslip.monthlyBasicSalary, 'f', 2).toStdString().c_str());
        html.replace("{{Allowances}}", QString::number(payslip.monthlyAllowances, 'f', 2).toStdString().c_str());
        html.replace("{{OvertimePay}}", QString::number(payslip.overTimePay, 'f', 2).toStdString().c_str());
        html.replace("{{Others}}", QString::number(payslip.adjustments, 'f', 2).toStdString().c_str());
        html.replace("{{GrossPay}}", QString::number(payslip.grossIncome, 'f', 2).toStdString().c_str());
        html.replace("{{SSS}}", QString::number(payslip.sssPremium, 'f', 2).toStdString().c_str());
        html.replace("{{PHIC}}", QString::number(payslip.philHealthPremium, 'f', 2).toStdString().c_str());
        html.replace("{{HDMF}}", QString::number(payslip.hdmfPremium, 'f', 2).toStdString().c_str());
        html.replace("{{WithholdingTaxes}}", QString::number(payslip.withHoldingTax, 'f', 2).toStdString().c_str());
        html.replace("{{TotalDeductions}}", QString::number(payslip.totalDeductions, 'f', 2).toStdString().c_str());
        html.replace("{{NetPay}}", QString::number(payslip.netPay, 'f', 2).toStdString().c_str());
    }
    else
    {
        qDebug() << "Failed to open payslip HTML resource!";
    }
    return html;
}

void PayslipWidget::onPreviewClicked()
{
    QString selectedEmployeeId = ui->employeeComboBox->currentData().toString();
    PayrollCalculationResults payrollForEmployee = getPayslipForEmployeeAndPeriod(selectedEmployeeId);
    QString html = htmlFileToQString(payrollForEmployee);
    PayslipPreviewDialog preview(html);
    preview.exec();
}

void PayslipWidget::onGenerateAllClicked()
{
    auto allEmployees = AppContext::instance().employeeService().getAllEmployees();
    if (!allEmployees.empty())
    {
        QString folderPath = QFileDialog::getExistingDirectory(this, "Select Folder to Save PDFs", QDir::homePath());
        for (auto &emp : allEmployees)
        {
            QString selectedEmployeeId = QString::fromStdString(emp.employeeId);
            QString tempPath = generatePdfFromHtmlInMemory(selectedEmployeeId);
            QString fileName = QFileInfo(tempPath).fileName();
            QString realPdfPath = QDir(folderPath).filePath(fileName);
            if (!realPdfPath.endsWith(".pdf", Qt::CaseInsensitive))
            {
                realPdfPath += ".pdf";
            }
            QPrinter printer(QPrinter::HighResolution);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(realPdfPath);

            printer.setPageSize(QPageSize(QPageSize::A4));
            printer.setPageMargins(QMarginsF(15, 15, 15, 15));
            pdfDoc.print(&printer);
            QFile::remove(tempPath);
        }
    }
}

void PayslipWidget::sendEmail(const QString &pdfPath)
{
    /// TODO: read PDF file as binary
    /// Doc: https://doc.qt.io/qt-6/qfile.html or std::ifstream

    /// TODO: convert file to QByteArray safely
    /// Doc: https://doc.qt.io/qt-6/qbytearray.html

    /// TODO: base64 encode the data
    /// Doc: https://doc.qt.io/qt-6/qbytearray.html#toBase64

    /// TODO: connect to SMTP server (SSL)
    /// Doc: https://doc.qt.io/qt-6/qsslsocket.html

    /// TODO: send EHLO, AUTH LOGIN, MAIL FROM, RCPT TO, DATA
    /// Doc: https://www.rfc-editor.org/rfc/rfc5321.html

    /// TODO: construct MIME message with text + attachment
    /// Docs:
    /// MIME standard: https://www.rfc-editor.org/rfc/rfc2045
    /// Email format: https://www.rfc-editor.org/rfc/rfc5322.html

    /// TODO: split base64 into 76-char lines
    /// Doc: https://www.rfc-editor.org/rfc/rfc2045.html#section-6.8

    /// TODO: send message over socket
    /// Doc: https://doc.qt.io/qt-6/qsslsocket.html

    /// TODO: QUIT and disconnect socket
    /// Doc: https://doc.qt.io/qt-6/qsslsocket.html

    /// TODO: delete temp PDF if needed
    /// Doc: https://doc.qt.io/qt-6/qfile.html#remove
}

QString PayslipWidget::generatePdfFromHtmlInMemory(QString &selectedEmployeeId)
{

    PayrollCalculationResults payrollForEmployee = getPayslipForEmployeeAndPeriod(selectedEmployeeId);
    QString suggestedName = getSuggestedFileName(payrollForEmployee);
    pdfDoc.setHtml(htmlFileToQString(payrollForEmployee));

    // Save to a temporary file
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/" + suggestedName + ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(tempPath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    pdfDoc.print(&printer);

    return tempPath;
}

QString PayslipWidget::getSuggestedFileName(PayrollCalculationResults &payslip)
{
    std::string payPeriodHalf = (payslip.payPeriodHalf == 1) ? "FirstHalf" : "SecondHalf";
    std::string baseFileName = payslip.payPeriodText;
    std::replace(baseFileName.begin(), baseFileName.end(), ' ', '_');
    QString suggestedFileName = QString::fromStdString(baseFileName + "_" + payPeriodHalf + "_" + payslip.employeeId);
    return suggestedFileName;
}

void PayslipWidget::handleEmployeeOrUpload(ActionType action)
{

    QString option;
    if (action == ActionType::PrintOrSave)
        option = "Selected Employee";
    else
    {
        option = showOptionDialog("Print Payslip", "Select Source of Payslip to Print: ", "Selected Employee", "Upload");
    }
    QString pdfPath{};
    if (option == "Selected Employee")
    {
        // creating the pdf in memory first,
        QString selectedEmployeeId = ui->employeeComboBox->currentData().toString();
        auto payrollInfo = getPayslipForEmployeeAndPeriod(selectedEmployeeId);
        QString suggestedFileName = getSuggestedFileName(payrollInfo);
        QString pdfPath = generatePdfFromHtmlInMemory(selectedEmployeeId);

        if (action == ActionType::PrintOrSave)
        {
            QString choice = showOptionDialog("Print or Save", "Save as PDF or Print? ", "Save As PDF", "Print");

            if (choice == "Save As PDF")
            {

                QString filePath = QFileDialog::getSaveFileName(this, "Save PDF", suggestedFileName, "PDF Files (*.pdf)");
                if (!filePath.isEmpty())
                {
                    if (!filePath.endsWith(".pdf", Qt::CaseInsensitive))
                    {
                        filePath += ".pdf";
                    }
                    QPrinter printer(QPrinter::HighResolution);
                    printer.setOutputFormat(QPrinter::PdfFormat);
                    printer.setOutputFileName(filePath);

                    printer.setPageSize(QPageSize(QPageSize::A4));
                    printer.setPageMargins(QMarginsF(15, 15, 15, 15));
                    pdfDoc.print(&printer);
                }
            }
            else if (choice == "Print")
            {
                QPrinter printer;
                QPrintDialog printDialog(&printer, this);
                if (printDialog.exec() == QDialog::Accepted)
                {
                    pdfDoc.print(&printer);
                }
            }
        }
        else if (action == ActionType::Email)
        {
            sendEmail(pdfPath);
            QFile::remove(pdfPath);
        }
    }
    else if (option == "Upload")
    {
        QString filePath = QFileDialog::getOpenFileName(this, "Select PDF", QString(), "PDF Files (*.pdf)");
        if (filePath.isEmpty())
            return;
        if (action == ActionType::Email)
        {
            sendEmail(filePath);
        }
    }
}

void PayslipWidget::onGenerateOneOrPrint()
{
    handleEmployeeOrUpload(ActionType::PrintOrSave);
}

void PayslipWidget::onEmailClicked()
{
    handleEmployeeOrUpload(ActionType::Email);
}
QString PayslipWidget::showOptionDialog(QString title, QString text, QString opt1, QString opt2)
{
    QMessageBox msgBox{this};
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    QPushButton *opt1Btn = msgBox.addButton(opt1, QMessageBox::AcceptRole);
    QPushButton *opt2Btn = msgBox.addButton(opt2, QMessageBox::AcceptRole);
    msgBox.exec();
    QString result{};
    if (msgBox.clickedButton() == opt1Btn)
    {
        return opt1;
    }
    else if (msgBox.clickedButton() == opt2Btn)
    {
        return opt2;
    }
    return "";
}
