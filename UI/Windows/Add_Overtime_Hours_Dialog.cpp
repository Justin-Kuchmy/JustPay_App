#include "include/Generated/ui_add_overtime_hours_dialog.h"
#include "UI/Add_Overtime_Hours_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

AddOvertimeHoursDialog::AddOvertimeHoursDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddOvertimeHoursDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddOvertimeHoursDialog::onOkayClicked); 
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddOvertimeHoursDialog::onCancelClicked);
};

AddOvertimeHoursDialog::~AddOvertimeHoursDialog()
{
    delete ui;
};

void AddOvertimeHoursDialog::onOkayClicked()
{
    std::string result = std::format("{{'regular':{},'rest_day':{},'rest_day_plus':{},'legal_holiday':{},'legal_holiday_plus':{},'special_holiday':{},'special_holiday_plus':{},'rest_plus_legal':{},'rest_plus_special':{},'night_shift_diff':{} }}",
        ui->regularSpinBox->value(),
        ui->restDaySpinBox->value(),
        ui->restDay8HrSpinBox->value(),
        ui->legalHolidaySpinBox->value(),
        ui->legalHoliday8hrSpinBox->value(),
        ui->specialHolidaySpinBox->value(),
        ui->specialHoliday8hrSpinBox->value(),
        ui->restDayAndLegalSpinBox->value(),
        ui->restDayAndSpecialSpinBox->value(),
        ui->nightShiftSpinBox->value()
    );
    this->jsonString = result;
    accept();
};

void AddOvertimeHoursDialog::onCancelClicked()
{
    reject();
};

std::string AddOvertimeHoursDialog::getJsonString()
{
    return this->jsonString;
}

