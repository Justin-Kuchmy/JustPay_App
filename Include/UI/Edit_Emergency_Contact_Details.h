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
    explicit EditEmergencyContactDetails(QWidget *parent = nullptr);
    ~EditEmergencyContactDetails();
    void setContactContext(std::optional<Contact> &contact);
private slots:
    void onSaveClicked();
private:
    Ui::EditEmergencyContactDetails *ui;
    std::unique_ptr<Contact> m_Contact;
};

#endif