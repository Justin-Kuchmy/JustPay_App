#ifndef DEFINEPAYROLLDIALOG_H
#define DEFINEPAYROLLDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class DefinePayrollFormulaDialog;
}

class DefinePayrollFormulaDialog : public QDialog {
    Q_OBJECT

public:
    explicit DefinePayrollFormulaDialog(QWidget *parent = nullptr);
    ~DefinePayrollFormulaDialog();
    
private slots:


private:
    Ui::DefinePayrollFormulaDialog *ui;
};


#endif