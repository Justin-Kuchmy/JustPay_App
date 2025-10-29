#ifndef EDITEMERGENCYCONTACTDETAILS_H
#define EDITEMERGENCYCONTACTDETAILS_H

#include <QDialog>
#include "../Include/Models/DataObjects.h"
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class EditEmergencyContactDetails;
}

class EditEmergencyContactDetails : public QWidget {
    Q_OBJECT
public:
    explicit EditEmergencyContactDetails(Employee &a_Employee, QWidget *parent = nullptr);
    ~EditEmergencyContactDetails();
    void setEmployeeContext();
private slots:

private:
    Ui::EditEmergencyContactDetails *ui;
    Employee &a_Employee;
};

#endif