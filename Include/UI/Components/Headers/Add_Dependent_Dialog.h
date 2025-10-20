#ifndef DEPENDENTDIALOG_H
#define DEPENDENTDIALOG_H

#include <QDialog>
#include "../Include/Entities/DataObjects.h"

namespace Ui {
class AddDependentDialog;
}

class AddDependentDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddDependentDialog(QWidget *parent = nullptr);
    ~AddDependentDialog();
    
private slots:


private:
    Ui::AddDependentDialog *ui;
};


#endif