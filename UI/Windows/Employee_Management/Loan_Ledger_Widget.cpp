#include "include/Generated/ui_loan_ledger_widget.h"
#include "UI/Employee_Management/Loan_Ledger_Widget.h"
#include "Services/AppContext.h"
#include "UI/Employee_Management/Add_Loan_Ledger_dialog.h"
#include "Utils/DialogFactory.h"

LoanLedgerWidget::LoanLedgerWidget(QWidget *parent) : QWidget(parent), ui(new Ui::LoanLedgerWidget),
                                                      m_LoanLedger{}, employeeId{}
{
    ui->setupUi(this);
    ui->loanLedgerSplitter->setSizes({300, 900});
    ui->loanTypeComboBox->addItems({QString::fromStdString(""),
                                    QString::fromStdString(loantype_to_string(LoanType::HDMF_Salary_Loan)),
                                    QString::fromStdString(loantype_to_string(LoanType::HDMF_Housing_Loan)),
                                    QString::fromStdString(loantype_to_string(LoanType::HDMF_Calamity_Loan)),
                                    QString::fromStdString(loantype_to_string(LoanType::SSS_Salary_Loan)),
                                    QString::fromStdString(loantype_to_string(LoanType::SSS_Calamity_Loan)),
                                    QString::fromStdString(loantype_to_string(LoanType::Personal_Cash_Advance)),
                                    QString::fromStdString(loantype_to_string(LoanType::Other))});

    ui->principalAmountSpinBox->setRange(0.00, 1'000'000'000.00);
    ui->principalAmountSpinBox->setDecimals(2);
    ui->principalAmountSpinBox->setSingleStep(1000.00);

    ui->loanListScrollArea->setWidgetResizable(true);

    connect(ui->saveButton, &QPushButton::clicked, this, &LoanLedgerWidget::onSaveClicked);
    connect(ui->addButton, &QPushButton::clicked, this, &LoanLedgerWidget::onAddClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &LoanLedgerWidget::onDeleteClicked);
    connect(ui->principalAmountSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), this, &LoanLedgerWidget::updateDeductionPerPayroll);
    connect(ui->amortSpinBox, qOverload<int>(&QSpinBox::valueChanged), this, &LoanLedgerWidget::updateDeductionPerPayroll);
    connect(ui->firstHalfCheckBox, &QCheckBox::toggled, this, &LoanLedgerWidget::updateDeductionPerPayroll);
    connect(ui->secondHalfCheckBox, &QCheckBox::toggled, this, &LoanLedgerWidget::updateDeductionPerPayroll);
};

LoanLedgerWidget::~LoanLedgerWidget()
{
    delete ui;
};

void LoanLedgerWidget::setLoanLedgerContext(std::vector<LoanLedger> *vectorPtr, std::string &employeeId)
{
    if (!vectorPtr)
        return;
    this->loanLedgers = vectorPtr;
    this->employeeId = employeeId;
    this->populateLoanList();
    this->setDefaults();
}

void LoanLedgerWidget::populateLoanList()
{

    auto *contentLayout = qobject_cast<QVBoxLayout *>(ui->scrollAreaWidgetContents->layout());
    if (!contentLayout)
    {
        contentLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
        contentLayout->setContentsMargins(0, 0, 0, 0);
        contentLayout->setSpacing(5);
    }

    // Clear previous items
    QLayoutItem *item;
    while ((item = contentLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

    // Add new loan items
    for (size_t i{}; i < loanLedgers->size(); ++i)
    {
        const auto &lled = loanLedgers->at(i);
        QString label = QString("%1 %2").arg(lled.loanLedgerId).arg(QString::fromStdString(loantype_to_string(static_cast<LoanType>(lled.loanType))));

        QPushButton *lledButton = new QPushButton(label);
        lledButton->setCursor(Qt::PointingHandCursor);
        contentLayout->addWidget(lledButton);

        connect(lledButton, &QPushButton::clicked, this, [this, i]()
                { this->onLoanSelected(i); });
    }

    contentLayout->addStretch();
};

void LoanLedgerWidget::onLoanSelected(size_t index)
{
    if (!loanLedgers || index >= loanLedgers->size())
        return;
    this->selectedLoanLedger = &this->loanLedgers->at(index);
    if (!this->selectedLoanLedger)
        return;

    ui->principalAmountSpinBox->blockSignals(true);
    ui->amortSpinBox->blockSignals(true);
    ui->firstHalfCheckBox->blockSignals(true);
    ui->secondHalfCheckBox->blockSignals(true);

    ui->loanTypeComboBox->setCurrentIndex(static_cast<int>(this->selectedLoanLedger->loanType) + 1);
    ui->principalAmountSpinBox->setValue(this->selectedLoanLedger->principalAmount);

    const Date &d = this->selectedLoanLedger->loanDate;
    ui->loanDateEdit->setDate(QDate(d.year, d.month, d.day));

    ui->amortSpinBox->setValue(this->selectedLoanLedger->NumOfAmortizations);

    ui->firstHalfCheckBox->setChecked(this->selectedLoanLedger->deductionFirstHalf);
    ui->secondHalfCheckBox->setChecked(this->selectedLoanLedger->deductionSecondHalf);

    // Calculate per payroll deduction
    int paymentsPerMonth = (this->selectedLoanLedger->deductionFirstHalf ? 1 : 0) +
                           (this->selectedLoanLedger->deductionSecondHalf ? 1 : 0);

    double perPayroll = 0.0;
    if (paymentsPerMonth > 0 && this->selectedLoanLedger->NumOfAmortizations > 0)
    {
        perPayroll = this->selectedLoanLedger->principalAmount / this->selectedLoanLedger->NumOfAmortizations / paymentsPerMonth;
    }
    ui->deductPayrollValue->setText(QString::number(perPayroll, 'f', 2));

    ui->statusValue->setChecked(this->selectedLoanLedger->status);

    // Unblock signals after population
    ui->principalAmountSpinBox->blockSignals(false);
    ui->amortSpinBox->blockSignals(false);
    ui->firstHalfCheckBox->blockSignals(false);
    ui->secondHalfCheckBox->blockSignals(false);
};

void LoanLedgerWidget::updateDeductionPerPayroll()
{
    double &principal = this->selectedLoanLedger->principalAmount;
    int &amortizations = this->selectedLoanLedger->NumOfAmortizations;
    bool &firstHalf = this->selectedLoanLedger->deductionFirstHalf;
    bool &secondHalf = this->selectedLoanLedger->deductionSecondHalf;
    double &perPayroll = this->selectedLoanLedger->deductionsPerPayroll;

    principal = ui->principalAmountSpinBox->value();
    amortizations = ui->amortSpinBox->value();
    firstHalf = ui->firstHalfCheckBox->isChecked();
    secondHalf = ui->secondHalfCheckBox->isChecked();

    int paymentsPerMonth = 0;
    if (firstHalf)
        paymentsPerMonth++;
    if (secondHalf)
        paymentsPerMonth++;

    if (paymentsPerMonth > 0 && amortizations > 0 && principal > 0.0)
    {
        perPayroll = principal / amortizations / paymentsPerMonth;
    }

    ui->deductPayrollValue->setText(QString::number(perPayroll, 'f', 2));
}

void LoanLedgerWidget::setDefaults()
{
    ui->loanTypeComboBox->setCurrentIndex(0);
    ui->principalAmountSpinBox->setValue(0.0);
    ui->loanDateEdit->setDate(QDate::fromString(QString::fromStdString("1900-01-01"), "yyyy-MM-dd"));
    ui->amortSpinBox->setValue(0);
    ui->firstHalfCheckBox->setChecked(0);
    ui->secondHalfCheckBox->setChecked(0);
    ui->deductPayrollValue->setText("Select a Loan Ledger");
    ui->statusValue->setChecked(0);
}

void LoanLedgerWidget::onSaveClicked()
{
    if (AppContext::instance().loanLedgerService().updateLoanLedger(*selectedLoanLedger))
    {
        QMessageBox::information(
            this,
            "Loan Ledger Updated",
            "Data has been saved successfully.");
    }
};

void LoanLedgerWidget::onAddClicked()
{
    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("add_loan_ledger");
    auto loanLedger = dynamic_cast<AddLoanLedgerDialog *>(dlg);
    if (loanLedger && loanLedger->exec() == QDialog::Accepted)
    {
        m_LoanLedger = loanLedger->getLoanLedgerData();
        m_LoanLedger.employeeId = this->employeeId;
        int newLoanLedgerId = AppContext::instance().loanLedgerService().addLoanLedger(this->m_LoanLedger);
        if (newLoanLedgerId > 0)
        {
            this->m_LoanLedger.loanLedgerId = newLoanLedgerId;
            this->loanLedgers->push_back(this->m_LoanLedger);
            this->populateLoanList();
        }
        else
        {
            QMessageBox::warning(
                this,
                "Add LoanLedger",
                "Failed to add Loan Ledger");
        }
    }
};

void LoanLedgerWidget::onDeleteClicked()
{
    auto reply = QMessageBox::question(this, "Delete Loan Ledger", "Really Delete this Loan Ledger?", QMessageBox::Yes, QMessageBox::Cancel);
    if (reply == QMessageBox::Yes)
    {
        if (AppContext::instance().loanLedgerService().deleteLoanLedger(selectedLoanLedger->loanLedgerId))
        {
            QMessageBox::information(
                this,
                "Delete LoanLedger",
                "Loan Ledger Deleted");
        }
        else
        {
            QMessageBox::warning(
                this,
                "Delete LoanLedger",
                "Loan Ledger deletion failed");
        }
        this->populateLoanList();
    }
};

QString LoanLedgerWidget::getSelectedLoan() const
{
    return QString::fromStdString("");
};
