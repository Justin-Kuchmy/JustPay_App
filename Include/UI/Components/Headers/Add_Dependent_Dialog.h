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

    Dependent getDependentData() const;
    
private slots:
    void accept() override;
    void reject() override;

    void on_nameLineEdit_textChanged(const QString &text);
    void on_relationLineEdit_textChanged(const QString &text);
    void on_bdayDateEdit_dateChanged(const QDate &date);

private:
    Ui::AddDependentDialog *ui;
    Dependent m_dependent;
};


#endif