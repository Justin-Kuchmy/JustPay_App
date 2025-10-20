#ifndef ADDLOANLEDGERDIALOG_H
#define ADDLOANLEDGERDIALOG_H


#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AddLoanLedger;
}

class AddLoanLedger : public QDialog {
    Q_OBJECT

public:
    explicit AddLoanLedger(QWidget *parent = nullptr);
    ~AddLoanLedger();
    
private slots:


private:
    Ui::AddLoanLedger *ui;
};


#endif