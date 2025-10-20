#ifndef SETPAYROLLPERIODDIALOG_H
#define SETPAYROLLPERIODDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class SetPayrollPeriodDialog;
}

class SetPayrollPeriodDialog : public QDialog {
    Q_OBJECT

public:
    explicit SetPayrollPeriodDialog(QWidget *parent = nullptr);
    ~SetPayrollPeriodDialog();
    
private slots:


private:
    Ui::SetPayrollPeriodDialog *ui;
};


#endif