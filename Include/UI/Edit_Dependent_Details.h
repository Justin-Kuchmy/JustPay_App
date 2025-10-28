#ifndef EDITDEPENDENTDETAILS_H
#define EDITDEPENDENTDETAILS_H

#include <QDialog>
#include "../Include/Models/DataObjects.h"
#include <QListWidgetItem>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>

namespace Ui {
class EditDependentDetails;
}

class EditDependentDetails : public QWidget {
    Q_OBJECT
public:
    explicit EditDependentDetails(QWidget *parent = nullptr);
    ~EditDependentDetails();
    void setEmployeeContext();
private slots:

private:   
    Ui::EditDependentDetails *ui;
    // Employee &a_Employee;
};

#endif