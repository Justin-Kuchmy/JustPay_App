#ifndef EMPLOYEESELECTDIALOG_H
#define EMPLOYEESELECTDIALOG_H


#include <QDialog>
#include "../Include/Entities/DataObjects.h"
#include <qt5/QtWidgets/qlistwidget.h>


namespace Ui {
class EmployeeSelectDialog;
}

class EmployeeSelectDialog : public QDialog {
    Q_OBJECT

public:
    explicit EmployeeSelectDialog(QWidget *parent = nullptr);
    ~EmployeeSelectDialog();
    QString getSelectedEmployee() const;
    std::vector<Employee>* empList;
    void populateEmployeeList();
    
private slots:
    void on_buttonBox_accepted();
    void onEmployeeSelected(QListWidgetItem* item);

private:
    Ui::EmployeeSelectDialog *ui;
    QString selectedEmployee;
};


#endif