

#include "include/Generated/ui_add_time_log_dialog.h"
#include "UI/Add_Time_Log_Dialog.h"
#include "UI/Add_Overtime_Hours_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"

AddTimeLogDialog::AddTimeLogDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddTimeLogDialog)
{
    ui->setupUi(this);
    connect(ui->selectOvertimeType,&QPushButton::clicked,this,&AddTimeLogDialog::onSelectTypeClicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddTimeLogDialog::onOkayClicked); //to close myself
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddTimeLogDialog::onCancelClicked);
};

AddTimeLogDialog::~AddTimeLogDialog()
{
    delete ui;
};

void AddTimeLogDialog::onOkayClicked()
{
    accept();
};

void AddTimeLogDialog::onCancelClicked()
{
    reject();
};

void AddTimeLogDialog::onSelectTypeClicked()
{
    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("Add_Overtime_Hours");
    auto overtimeHours = dynamic_cast<AddOvertimeHoursDialog*>(dlg);
    if(overtimeHours && overtimeHours->exec() == QDialog::Accepted)
    {

    }
}

