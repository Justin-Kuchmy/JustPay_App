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
    explicit EditEmployeeDetails(QWidget *parent = nullptr);
    ~EditEmployeeDetails();
    EditEmployeeDetails(const EditEmployeeDetails&) = delete; 
    EditEmployeeDetails& operator=(const EditEmployeeDetails&) = delete;
    void setEmployeeContext(Employee &m_Employee);
private slots:
    void onSaveClicked();
private:
    Ui::EditEmployeeDetails *ui;
    std::unique_ptr<Employee> m_Employee;
};

#endif