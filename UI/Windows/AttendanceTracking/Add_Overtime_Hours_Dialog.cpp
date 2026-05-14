#include "include/Generated/ui_add_overtime_hours_dialog.h"
#include "UI/AttendanceTracking/Add_Overtime_Hours_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

AddOvertimeHoursDialog::AddOvertimeHoursDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddOvertimeHoursDialog), jsonString{}
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
    std::ostringstream oss;
    oss << "{'regular':" << ui->regularSpinBox->value()
        << ",'rest_day':" << ui->restDaySpinBox->value()
        << ",'rest_day_plus':" << ui->restDay8HrSpinBox->value()
        << ",'legal_holiday':" << ui->legalHolidaySpinBox->value()
        << ",'legal_holiday_plus':" << ui->legalHoliday8hrSpinBox->value()
        << ",'special_holiday':" << ui->specialHolidaySpinBox->value()
        << ",'special_holiday_plus':" << ui->specialHoliday8hrSpinBox->value()
        << ",'rest_plus_legal':" << ui->restDayAndLegalSpinBox->value()
        << ",'rest_plus_special':" << ui->restDayAndSpecialSpinBox->value()
        << ",'night_shift_diff':" << ui->nightShiftSpinBox->value()
        << "}";
    this->jsonString = oss.str();
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
