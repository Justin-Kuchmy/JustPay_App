#ifndef EmployeeDetailsWidget_H
#define EmployeeDetailsWidget_H

#include <QDialog>
#include "../Include/Models/DataObjects.h"
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class EditEmployeeDetails;
}

class EditEmployeeDetails : public QWidget {
    Q_OBJECT
public:
    explicit EditEmployeeDetails(Employee &a_Employee, QWidget *parent = nullptr);
    ~EditEmployeeDetails();
    void setEmployeeContext();
private slots:
    void onSaveClicked();
private:
    Ui::EditEmployeeDetails *ui;
    Employee &a_Employee;
};

#endif