#ifndef LOANLEDGERWIDGET_H
#define LOANLEDGERWIDGET_H
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include "../Include/Models/DataObjects.h"

namespace Ui {
class LoanLedgerWidget;
}
class LoanLedgerWidget : public QWidget {
    Q_OBJECT
public:
    LoanLedgerWidget(QWidget *parent = nullptr);
    ~LoanLedgerWidget();
private slots:
    void onSaveClicked();
    void onDeleteClicked();
    void onTogleStatusClicked();
    void onLoanSelected(QString& empID);
    QString getSelectedLoan() const;
private:
    Ui::LoanLedgerWidget *ui;
};
#endif