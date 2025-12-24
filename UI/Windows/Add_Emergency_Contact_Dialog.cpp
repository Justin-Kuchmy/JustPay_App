#include "include/Generated/ui_add_emergency_contact.h"
#include "UI/Add_Emergency_Contact_Dialog.h"
#include "Services/AppContext.h"

EmergencyContactDialog::EmergencyContactDialog(QWidget *parent): QDialog(parent), ui(new Ui::EmergencyContactDialog), m_Contact{}
{
    ui->setupUi(this); 
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept); //disable auto close after accepted
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
    if(!m_Contact.name.empty() && !m_Contact.relation.empty() && !m_Contact.address.empty() && !m_Contact.contactNo.empty())
    {
        accept();
    }
    else
    {
        QMessageBox::warning(
            this,
            "Add Contact Failed",
            "Unable to save dependent.\nBlank Fields"
        );
        this->setResult(QDialog::Rejected);
    }
};

void EmergencyContactDialog::onCancelClicked()
{
    reject();
};

Contact EmergencyContactDialog::getContactData() const
{
    return this->m_Contact;
}