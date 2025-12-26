#ifndef EMERGENCYCONTACTDIALOG_H
#define EMERGENCYCONTACTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include "../Include/Models/DataObjects.h"

namespace Ui {
class EmergencyContactDialog;
}

class EmergencyContactDialog : public QDialog {
    Q_OBJECT

public:
    explicit EmergencyContactDialog(QWidget *parent = nullptr);
    ~EmergencyContactDialog();
    EmergencyContactDialog(const EmergencyContactDialog&) = delete; 
    EmergencyContactDialog& operator=(const EmergencyContactDialog&) = delete;

    Contact getContactData() const;
    
private slots:
    void onOKClicked();
    void onCancelClicked();

private:
    Ui::EmergencyContactDialog *ui;
    Contact m_Contact;
};


#endif