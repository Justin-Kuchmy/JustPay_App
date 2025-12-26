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
    AddEmployeeDialog(const AddEmployeeDialog&) = delete; 
    AddEmployeeDialog& operator=(const AddEmployeeDialog&) = delete;
private slots:
    void onOKClicked();
    void onCancelClicked();
    void openAddContactDialog();
    void openAddDependentDialog();
private:
    Ui::AddEmployeeDialog *ui;
    QVector<Employee>* employees;
    Employee a_Employee;
    Contact m_Contact;
    Dependent m_Dependent;
};


#endif