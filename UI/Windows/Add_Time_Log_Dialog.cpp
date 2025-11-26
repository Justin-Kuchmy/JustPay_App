

#include "include/Generated/ui_add_time_log_dialog.h"
#include "UI/Add_Time_Log_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"

AddTimeLogDialog::AddTimeLogDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddTimeLogDialog)
{
    ui->setupUi(this);
};

AddTimeLogDialog::~AddTimeLogDialog()
{
    delete ui;
};

void AddTimeLogDialog::onOkayClicked()
{

};

void AddTimeLogDialog::onCancelClicked()
{

};

