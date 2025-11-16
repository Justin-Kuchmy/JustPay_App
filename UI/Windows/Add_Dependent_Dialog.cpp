#include "include/Generated/ui_add_dependent.h"
#include "UI/Add_Dependent_Dialog.h"
#include "Services/AppContext.h"

AddDependentDialog::AddDependentDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddDependentDialog)
{
    ui->setupUi(this); 
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept); //disable auto close after accepted
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddDependentDialog::onOKClicked); //to close myself
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddDependentDialog::onCancelClicked);

}

AddDependentDialog::~AddDependentDialog()
{
    delete ui;
};

void AddDependentDialog::onOKClicked()
{
    m_Dependent.name = ui->nameLineEdit->text().toStdString();
    m_Dependent.relation = ui->relationLineEdit->text().toStdString();
    m_Dependent.birthday = Date::fromString(ui->bdayDateEdit->date().toString("yyyy-MM-dd").toStdString());
    auto today = Date::getTodayDate();
    if (m_Dependent.birthday > today) {
        LOG_DEBUG("[AddDependentDialog] record failed check birthday");
        QMessageBox::warning(
            this,
            "Add Dependent Failed",
            "Unable to save dependent.\nPlease check birthday"
        );
        this->setResult(QDialog::Rejected);
    }
    else
    {
        LOG_DEBUG("[AddDependentDialog] Dependent recorded");
        accept();
    }

};

void AddDependentDialog::onCancelClicked()
{
    reject();
};


Dependent AddDependentDialog::getDependentData() const
{
    return this->m_Dependent;
}