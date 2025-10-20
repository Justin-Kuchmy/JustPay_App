#ifndef COMPUTEPAYROLLDIALOG_H
#define COMPUTEPAYROLLDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class ComputePayrollDialog;
}

class ComputePayrollDialog : public QDialog {
    Q_OBJECT

public:
    explicit ComputePayrollDialog(QWidget *parent = nullptr);
    ~ComputePayrollDialog();
    
private slots:


private:
    Ui::ComputePayrollDialog *ui;
};


#endif