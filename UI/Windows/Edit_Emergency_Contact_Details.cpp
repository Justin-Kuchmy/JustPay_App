#include "include/Generated/ui_edit_emergency_contact_details.h"
#include "UI/Edit_Emergency_Contact_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#define DEBUG_LOGS
#include "Utils/Log.h"


EditEmergencyContactDetails::EditEmergencyContactDetails(QWidget *parent): QWidget(parent), ui(new Ui::EditEmergencyContactDetails)
{
    ui->setupUi(this); 
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditEmergencyContactDetails::onSaveClicked);

};

EditEmergencyContactDetails::~EditEmergencyContactDetails()
{
    delete ui;
};

void EditEmergencyContactDetails::setContactContext(std::optional<Contact>& contact)
{

    if(contact.has_value())
    {
        this->m_Contact = std::make_unique<Contact>(contact.value());
        ui->nameLineEdit->setText(QString::fromStdString(m_Contact->name));
        ui->relationLineEdit->setText(QString::fromStdString(m_Contact->relation));
        ui->addressLineEdit->setText(QString::fromStdString(m_Contact->address));
        ui->contactNumLineEdit->setText(QString::fromStdString(m_Contact->contactNo));
    }

};


void EditEmergencyContactDetails::onSaveClicked()
{
    m_Contact->name = ui->nameLineEdit->text().toStdString();
    m_Contact->relation = ui->relationLineEdit->text().toStdString();
    m_Contact->address = ui->addressLineEdit->text().toStdString();
    m_Contact->contactNo = ui->contactNumLineEdit->text().toStdString();
    if(AppContext::instance().emergencyContactService().updateEmergencyContact(*m_Contact))
    {
        LOG_DEBUG(m_Contact->name << " updated!");
    }
};
