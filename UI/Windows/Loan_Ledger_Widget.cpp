#include "include/Generated/ui_loan_ledger_widget.h"
#include "UI/Loan_Ledger_Widget.h"


LoanLedgerWidget::LoanLedgerWidget(QWidget *parent): QWidget(parent), ui(new Ui::LoanLedgerWidget)
{
    ui->setupUi(this); 
};

LoanLedgerWidget::~LoanLedgerWidget()
{

};

void LoanLedgerWidget::onSaveClicked()
{

};

void LoanLedgerWidget::onDeleteClicked()
{

};

void LoanLedgerWidget::onTogleStatusClicked()
{

};

void LoanLedgerWidget::onLoanSelected(QString& empID)
{

};

QString LoanLedgerWidget::getSelectedLoan() const
{
    return QString::fromStdString("");
};
