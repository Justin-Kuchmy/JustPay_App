#ifndef LOANLEDGERWIDGET_H
#define LOANLEDGERWIDGET_H
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include "../Include/Models/DataObjects.h"

namespace Ui {
class LoanLedgerWidget;
}
class LoanLedgerWidget : public QWidget {
    Q_OBJECT
public:
    LoanLedgerWidget(QWidget *parent = nullptr);
    ~LoanLedgerWidget();
    LoanLedgerWidget(const LoanLedgerWidget&) = delete; 
    LoanLedgerWidget& operator=(const LoanLedgerWidget&) = delete;    
    void setLoanLedgerContext(std::vector<LoanLedger> *loanLedger, std::string& employeeId);
private slots:
    void onSaveClicked();
    void onAddClicked();
    void onDeleteClicked();
    void onLoanSelected(size_t i);
    QString getSelectedLoan() const;
    void updateDeductionPerPayroll();
    void setDefaults();
    void populateLoanList();
private:
    Ui::LoanLedgerWidget *ui;
    std::vector<LoanLedger>* loanLedgers = nullptr;
    LoanLedger* selectedLoanLedger = nullptr;
    LoanLedger m_LoanLedger;
    std::string employeeId;
    int8_t paymentsPerMonth{};

};
#endif