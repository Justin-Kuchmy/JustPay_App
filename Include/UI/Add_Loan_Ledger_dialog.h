#ifndef LOANLEDGERDIALOG_H
#define LOANLEDGERDIALOG_H 
#include <QDialog>
#include <QMessageBox>
#include "Services/AppContext.h"
#include "../Include/Models/DataObjects.h"

namespace Ui {
    class AddLoanLedgerDialog;
}

class AddLoanLedgerDialog: public QDialog {
Q_OBJECT

public: 
    explicit AddLoanLedgerDialog(QWidget *parent = nullptr);
    ~AddLoanLedgerDialog();

    LoanLedger getLoanLedgerData() const;

private slots: 
    void onOKClicked();
    void onCancelClicked();

private:
    Ui::AddLoanLedgerDialog *ui;
    LoanLedger m_LoanLedger; 
    std::string m_employeeId; 

};

#endif