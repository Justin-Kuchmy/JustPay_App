#include "include/Generated/ui_add_emergency_contact.h"
#include "UI/Add_Emergency_Contact_Dialog.h"

EmergencyContactDialog::EmergencyContactDialog(QWidget *parent): QDialog(parent), ui(new Ui::EmergencyContactDialog)
{
    ui->setupUi(this); 
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EmergencyContactDialog::onOKClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &EmergencyContactDialog::onCancelClicked);
}

EmergencyContactDialog::~EmergencyContactDialog()
{
    delete ui;
};

void EmergencyContactDialog::onOKClicked()
{
    m_Contact.name = ui->nameLineEdit->text().toStdString();
    m_Contact.relation= ui->relationLineEdit->text().toStdString();
    m_Contact.address= ui->addressLineEdit->text().toStdString();
    m_Contact.contactNo= ui->contactNumLineEdit->text().toStdString();
    accept();
};

void EmergencyContactDialog::onCancelClicked()
{
    reject();
};

Contact EmergencyContactDialog::getContactData() const
{
    return this->m_Contact;
}