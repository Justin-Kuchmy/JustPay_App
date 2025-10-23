#include "include/Generated/ui_add_emergency_contact.h"
#include "UI/Add_Emergency_Contact_Dialog.h"

EmergencyContactDialog::EmergencyContactDialog(QWidget *parent): QDialog(parent), ui(new Ui::EmergencyContactDialog)
{
    ui->setupUi(this); 
}

EmergencyContactDialog::~EmergencyContactDialog()
{
    delete ui;
};

void EmergencyContactDialog::onOKClicked()
{

};

void EmergencyContactDialog::onCancelClicked()
{

};