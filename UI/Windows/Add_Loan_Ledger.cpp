#include "include/Generated/ui_add_loan_ledger_dialog.h"
#include "UI/Add_Loan_Ledger_dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"


AddLoanLedgerDialog::AddLoanLedgerDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddLoanLedgerDialog),
    m_LoanLedger{}, m_employeeId{}
{
    ui->setupUi(this);
    ui->loanTypeComboBox->addItems({
        QString::fromStdString(""),
        QString::fromStdString(loantype_to_string(LoanType::HDMF_Salary_Loan)),
        QString::fromStdString(loantype_to_string(LoanType::HDMF_Housing_Loan)),
        QString::fromStdString(loantype_to_string(LoanType::HDMF_Calamity_Loan)),
        QString::fromStdString(loantype_to_string(LoanType::SSS_Salary_Loan)),
        QString::fromStdString(loantype_to_string(LoanType::SSS_Calamity_Loan)),
        QString::fromStdString(loantype_to_string(LoanType::Personal_Cash_Advance)),
        QString::fromStdString(loantype_to_string(LoanType::Other))
    });
    ui->principalAmountDoubleSpinBox->setRange(0.00, 1'000'000'000.00);
    ui->principalAmountDoubleSpinBox->setDecimals(2);
    ui->principalAmountDoubleSpinBox->setSingleStep(1000.00);
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept); //disable auto close after accepted
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddLoanLedgerDialog::onOKClicked); //to close myself
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddLoanLedgerDialog::onCancelClicked);
}
AddLoanLedgerDialog::~AddLoanLedgerDialog()
{
    delete ui;
}

void AddLoanLedgerDialog::onOKClicked()
{
    this->m_LoanLedger.loanType = static_cast<LoanType>(ui->loanTypeComboBox->currentIndex()-1);
    this->m_LoanLedger.principalAmount = ui->principalAmountDoubleSpinBox->value();
    this->m_LoanLedger.loanDate = Date::fromString(ui->loanDateEdit->date().toString("yyyy-MM-dd").toStdString());
    this->m_LoanLedger.NumOfAmortizations = ui->aportizationSpinBox->value();
    this->m_LoanLedger.deductionsPerPayroll = 0.0;
    this->m_LoanLedger.deductionFirstHalf = ui->firstHalfBox->isChecked();
    this->m_LoanLedger.deductionSecondHalf = ui->secondHalfBox->isChecked();
    this->m_LoanLedger.status = ui->processCheckBox->isChecked();
    accept();

    
}
void AddLoanLedgerDialog::onCancelClicked()
{
    reject();
}

LoanLedger AddLoanLedgerDialog::getLoanLedgerData() const
{
    return this->m_LoanLedger;
}