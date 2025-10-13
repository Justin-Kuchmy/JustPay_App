#ifndef EMPLOYEESELECTDIALOG_H
#define EMPLOYEESELECTDIALOG_H

// employee_select_dialog.h
#include <QDialog>


namespace Ui {
class EmployeeSelectDialog;
}

class EmployeeSelectDialog : public QDialog {
    Q_OBJECT

public:
    explicit EmployeeSelectDialog(QWidget *parent = nullptr);
    QString getSelectedEmployee() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EmployeeSelectDialog *ui;
    QString selectedEmployee;
};


#endif