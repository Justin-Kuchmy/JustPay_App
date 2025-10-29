#include "include/Generated/ui_add_dependent.h"
#include "UI/Add_Dependent_Dialog.h"

AddDependentDialog::AddDependentDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddDependentDialog)
{
    ui->setupUi(this); 
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddDependentDialog::onOKClicked);
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
    m_Dependent.birthday = from_string(ui->bdayDateEdit->date().toString("yyyy-MM-dd").toStdString());
    accept();
};

void AddDependentDialog::onCancelClicked()
{
    reject();
};


Dependent AddDependentDialog::getDependentData() const
{
    return this->m_Dependent;
}