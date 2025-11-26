

#include "include/Generated/ui_add_overtime_hours_dialog.h"
#include "UI/Add_Overtime_Hours_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

AddOvertimeHoursDialog::AddOvertimeHoursDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddOvertimeHoursDialog)
{
    ui->setupUi(this);
};

AddOvertimeHoursDialog::~AddOvertimeHoursDialog()
{
    delete ui;
};

void AddOvertimeHoursDialog::onOkayClicked()
{

};

void AddOvertimeHoursDialog::onCancelClicked()
{

};

