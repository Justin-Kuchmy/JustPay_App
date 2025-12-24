#include "include/Generated/ui_edit_dependent_details.h"
#include "UI/Edit_Dependent_Details.h"
#include "Services/EmployeeService.h"
#include "Repositories/EmployeeRepository.h"
#include "Services/AppContext.h"
#define DEBUG_LOGS
#include "Utils/Log.h"


EditDependentDetails::EditDependentDetails(QWidget *parent): QWidget(parent), ui(new Ui::EditDependentDetails)
{

    ui->setupUi(this); 
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditDependentDetails::onSaveClicked);

};

EditDependentDetails::~EditDependentDetails()
{
    delete ui;
};

void EditDependentDetails::setDependentContext(std::optional<Dependent>& dependent)
{   
    if(dependent.has_value())
    {
        this->m_Dependent = std::make_unique<Dependent>(dependent.value());
        ui->nameLineEdit->setText(QString::fromStdString(m_Dependent->name));
        ui->relationLineEdit->setText(QString::fromStdString(m_Dependent->relation));
        ui->bdayDateEdit->setDate(QDate::fromString(QString::fromStdString(m_Dependent->birthday.to_string())));
    }
};


void EditDependentDetails::onSaveClicked()
{

    m_Dependent->name = ui->nameLineEdit->text().toStdString();
    m_Dependent->relation = ui->relationLineEdit->text().toStdString();
    m_Dependent->birthday = Date::fromString(ui->bdayDateEdit->date().toString("yyyy-MM-dd").toStdString());
    if(AppContext::instance().dependentService().updateDependent(*m_Dependent))
    {
        LOG_DEBUG(m_Dependent->name << " updated!");
    }
};
