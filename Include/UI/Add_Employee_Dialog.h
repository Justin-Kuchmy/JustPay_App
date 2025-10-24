#ifndef ADDEMPLOYEEDIALOG_H
#define ADDEMPLOYEEDIALOG_H

#include <QDialog>
#include "../Include/Models/DataObjects.h"

namespace Ui {
class AddEmployeeDialog;
}

class AddEmployeeDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddEmployeeDialog(QWidget *parent = nullptr);
    ~AddEmployeeDialog();   
private slots:
    void onOKClicked();
    void onCancelClicked();
    void openAddContactDialog();
    void openAddDependentDialog();
private:
    Ui::AddEmployeeDialog *ui;
    Employee a_Employee;
    QVector<Employee>* employees;
};


#endif