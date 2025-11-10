#ifndef DEPENDENTDIALOG_H
#define DEPENDENTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "../Include/Models/DataObjects.h"

namespace Ui {
class AddDependentDialog;
}

class AddDependentDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddDependentDialog(QWidget *parent = nullptr);
    ~AddDependentDialog();

    Dependent getDependentData() const;
    
private slots:
    void onOKClicked();
    void onCancelClicked();

private:
    Ui::AddDependentDialog *ui;
    Dependent m_Dependent;
};


#endif