#include "include/Generated/ui_add_sql_description.h"
#include "UI/Add_Sql_Description.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

AddSqlDescriptionDialog::AddSqlDescriptionDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddSqlDescriptionDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddSqlDescriptionDialog::onSaveClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddSqlDescriptionDialog::getSqlDescription);
}

AddSqlDescriptionDialog::~AddSqlDescriptionDialog()
{
    delete ui;
}

void AddSqlDescriptionDialog::onSaveClicked()
{
    sqlDescription = ui->description->text().toStdString();
    accept();
};

std::string AddSqlDescriptionDialog::getSqlDescription()
{   
    return this->sqlDescription;
}

void AddSqlDescriptionDialog::onCancelClicked()
{
    reject();
};
