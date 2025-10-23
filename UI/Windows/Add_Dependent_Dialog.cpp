#include "include/Generated/ui_add_dependent.h"
#include "UI/Add_Dependent_Dialog.h"

AddDependentDialog::AddDependentDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddDependentDialog)
{
    ui->setupUi(this); 
}

AddDependentDialog::~AddDependentDialog()
{
    delete ui;
};

void AddDependentDialog::onOKClicked()
{

};

void AddDependentDialog::onCancelClicked()
{

};
