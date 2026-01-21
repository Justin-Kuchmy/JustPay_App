#include "include/Generated/ui_payslip_widget.h"
#include "UI/Payslip_Generation_Widget.h"
#include "Utils/Log.h"

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
    connect(ui->generateSelectedButton, &QPushButton::clicked, this, &PayslipWidget::onGenerateOneClicked);
    connect(ui->printButton, &QPushButton::clicked, this, &PayslipWidget::onPrintClicked);
    connect(ui->emailButton, &QPushButton::clicked, this, &PayslipWidget::onEmailClicked);
}

PayslipWidget::~PayslipWidget()
{
    delete ui;
}

void PayslipWidget::onGenerateAllClicked()
{
    auto allEmployees = AppContext::instance().employeeService().getAllEmployees();
    if (!allEmployees.empty())
    {
        for (auto &emp : allEmployees)
        {
            QString selectedEmployeeId = QString::fromStdString(emp.employeeId);
            const auto payrollOpt = AppContext::instance().payrollService().getPayrollByEmployeeAndPeriod(emp.employeeId, ui->payrollDateEdit->date().toString("MMMM yyyy").toStdString(), ui->comboPayrollPeriod->currentIndex() + 1);
            QString path{};
            if (payrollOpt.has_value())
            {

                path = generatePDF(payrollOpt.value());
                qDebug() << "Generated payslip saved to:" << path;
            }
        }
    }
}

void PayslipWidget::onGenerateOneClicked()
{

    QString selectedEmployeeId = ui->employeeComboBox->currentData().toString();

    const auto payrollOpt = AppContext::instance().payrollService().getPayrollByID(1);
    QString path{};
    if (payrollOpt.has_value())
    {

        path = generatePDF(payrollOpt.value());
        qDebug() << "Generated payslip saved to:" << path;
    }
    qDebug() << "Failed to Generated payslip for employee ID:" << selectedEmployeeId;
}

void PayslipWidget::onPrintClicked()
{
    // Option 1: Let the user select one or more existing PDF files and print them
    // Option 2: Generate the payslip for the selected employee on the fly and print it directly
    //  - Handle printer selection and page setup
    //  - Optionally show print success/failure
}

void PayslipWidget::onEmailClicked()
{
    // Option 1: Let the user select one or more existing PDF files and email them
    // Option 2: Generate the payslip for the selected employee on the fly and email it directly
    //  - Prepare email content (recipient, subject, body)
    //  - Attach the PDF(s)
    //  - Optionally handle success/failure
}

QString PayslipWidget::generatePDF(const PayrollCalculationResults &payslip, const QString &outputDir)
{
    QString defaultOutputDir = QDir(QCoreApplication::applicationDirPath()).filePath("../generated_payslips");

    QDir dir(defaultOutputDir);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    std::string payPeriodHalf = (payslip.payPeriodHalf == 1) ? "FirstHalf" : "SecondHalf";

    std::string forFileName = payslip.payPeriodText;
    std::replace(forFileName.begin(), forFileName.end(), ' ', '_');

    QString fileName = QString::fromStdString(forFileName + "_" + payPeriodHalf + "_" + payslip.employeeId) + ".pdf";
    QString fullPath = dir.filePath(fileName);

    qDebug() << "Generating PDF for" << QString::fromStdString(payslip.employeeId) << "at" << fullPath;

    QString html;

    QFile file(":/resources/docs/payslip.html");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        html = file.readAll();
        file.close();

        html.replace("{{PayPeriod}}", payslip.payPeriodText.c_str());
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

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fullPath);

    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    return fullPath;
}

void PayslipWidget::sendEmailWithAttachment(const QString &recipientEmail, const QString &attachmentPath, const QString &subject, const QString &body)
{
    qDebug() << "Sending email to" << recipientEmail << "with attachment" << attachmentPath;
}

void PayslipWidget::printPayslip(const PayrollCalculationResults &payslip)
{
    qDebug() << "Printing payslip for" << QString::fromStdString(payslip.fullName);
}

void PayslipWidget::printPayslip(const QString &pdfPath)
{
    qDebug() << "Printing payslip from PDF:" << pdfPath;
}
