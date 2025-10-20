/********************************************************************************
** Form generated from reading UI file 'employee_select_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EMPLOYEE_SELECT_DIALOG_H
#define UI_EMPLOYEE_SELECT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EmployeeSelectDialog
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelTitle;
    QLineEdit *searchBar;
    QListWidget *employeeList;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *EmployeeSelectDialog)
    {
        if (EmployeeSelectDialog->objectName().isEmpty())
            EmployeeSelectDialog->setObjectName("EmployeeSelectDialog");
        EmployeeSelectDialog->resize(300, 250);
        verticalLayout_3 = new QVBoxLayout(EmployeeSelectDialog);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        labelTitle = new QLabel(EmployeeSelectDialog);
        labelTitle->setObjectName("labelTitle");

        verticalLayout->addWidget(labelTitle);

        searchBar = new QLineEdit(EmployeeSelectDialog);
        searchBar->setObjectName("searchBar");

        verticalLayout->addWidget(searchBar);

        employeeList = new QListWidget(EmployeeSelectDialog);
        employeeList->setObjectName("employeeList");

        verticalLayout->addWidget(employeeList);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(EmployeeSelectDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setLayoutDirection(Qt::RightToLeft);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(EmployeeSelectDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, EmployeeSelectDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, EmployeeSelectDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(EmployeeSelectDialog);
    } // setupUi

    void retranslateUi(QDialog *EmployeeSelectDialog)
    {
        EmployeeSelectDialog->setWindowTitle(QCoreApplication::translate("EmployeeSelectDialog", "Select Employee", nullptr));
        labelTitle->setText(QCoreApplication::translate("EmployeeSelectDialog", "Select an employee:", nullptr));
        searchBar->setPlaceholderText(QCoreApplication::translate("EmployeeSelectDialog", "Search employees...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EmployeeSelectDialog: public Ui_EmployeeSelectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EMPLOYEE_SELECT_DIALOG_H
