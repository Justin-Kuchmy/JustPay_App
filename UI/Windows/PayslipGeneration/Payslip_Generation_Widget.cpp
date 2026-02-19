#include "include/Generated/ui_payslip_widget.h"
#include "UI/PayslipGeneration/Payslip_Generation_Widget.h"
#include "UI/PayslipGeneration/Payslip_Preview_Dialog.h"
#include "Utils/Log.h"
#include <QStandardPaths>
#include <curl/curl.h>
#include "Utils/AuthService.h"

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
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    connect(ui->emailButton, &QPushButton::clicked, this, &PayslipWidget::onEmailClicked);
    connect(ui->pdfPreviewButton, &QPushButton::clicked, this, &PayslipWidget::onPreviewClicked);
    connect(ui->employeeComboBox, &QComboBox::currentIndexChanged, this, &PayslipWidget::onComboBoxChanged);
}

void PayslipWidget::onComboBoxChanged()
{
    selectedEmployeeId = ui->employeeComboBox->currentData().toString();
    qDebug() << selectedEmployeeId;
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
            selectedEmployeeId = QString::fromStdString(emp.employeeId);
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
    qDebug() << "sending email.. ";

    std::optional<Employee> empOpt = AppContext::instance().employeeService().getEmployeeByID(selectedEmployeeId.toStdString());
    std::string emailString;
    std::string confMessage{};
    if (!empOpt.has_value())
    {
        confMessage = "Employee not found. Email was not sent.";
    }
    else
    {
        selectedEmployee = empOpt.value();
        emailString = "<" + selectedEmployee.personalEmail + ">";

        CURL *curl = curl_easy_init();
        curl_mime *mime = curl_mime_init(curl);
        curl_mimepart *part;
        EmailCrudentials details = AuthService::getInstance().createOrReadSettingsFile();

        const char *companyEmail = details.companyEmail.c_str();
        const char *appPassword = details.appPassword.c_str();

        curl_easy_setopt(curl, CURLOPT_USERNAME, companyEmail);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, appPassword);
        curl_easy_setopt(curl, CURLOPT_URL, "smtps://smtp.gmail.com:465");

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, companyEmail);
        struct curl_slist *recipients = nullptr;
        std::string emailString = "<" + selectedEmployee.personalEmail + ">";
        recipients = curl_slist_append(recipients, emailString.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        // email body
        part = curl_mime_addpart(mime);
        curl_mime_data(part, "Please find your payslip attached.", CURL_ZERO_TERMINATED);
        curl_mime_type(part, "text/plain");

        // pdf attachment
        part = curl_mime_addpart(mime);
        curl_mime_filedata(part, pdfPath.toStdString().c_str());
        curl_mime_type(part, "application/pdf");
        curl_mime_encoder(part, "base64");

        curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Subject: Payslip Test");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode result;
        result = curl_easy_perform(curl);
        if (result != CURLE_OK)
        {
            confMessage = "Email failed to send to " + selectedEmployee.personalEmail + "!";
        }
        else
        {
            confMessage = "Email Sent Successfully to " + selectedEmployee.personalEmail + "!";
        }

        // clean up
        curl_slist_free_all(recipients);
        curl_slist_free_all(headers);
        curl_mime_free(mime);
        curl_easy_cleanup(curl);
    }

    QMessageBox msgBox{this};
    msgBox.setFixedWidth(150);
    msgBox.setWindowTitle("Confirmation");

    msgBox.setText(QString::fromStdString(confMessage));
    msgBox.addButton("Okay", QMessageBox::AcceptRole);
    msgBox.exec();
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
        selectedEmployeeId = ui->employeeComboBox->currentData().toString();
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
