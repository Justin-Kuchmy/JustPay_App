

#include "include/Generated/ui_add_time_log_dialog.h"
#include "UI/AttendanceTracking/Add_Time_Log_Dialog.h"
#include "UI/AttendanceTracking/Add_Overtime_Hours_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"
#include "Services/AppContext.h"
#include <iterator>

AddTimeLogDialog::AddTimeLogDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AddTimeLogDialog),
                                                      jsonDataFromDialog{""}, allEmployees{}, selectedAttendanceLog{}, selectedEmployee{}
{
    ui->setupUi(this);
    connect(ui->selectOvertimeType, &QPushButton::clicked, this, &AddTimeLogDialog::onSelectTypeClicked);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &AddTimeLogDialog::onCancelClicked);
    connect(ui->empComboBox, &QComboBox::currentIndexChanged, this, &AddTimeLogDialog::onEmployeeSelected);
};

AddTimeLogDialog::~AddTimeLogDialog()
{
    delete ui;
};

void AddTimeLogDialog::getLogData(int logId)
{
    std::optional<AttendanceLog> optLog = AppContext::instance().attendanceLogService().getAttendanceLogByID(logId);

    if (optLog.has_value())
    {
        selectedAttendanceLog = optLog.value();

        auto it = std::find_if(allEmployees.begin(), allEmployees.end(), [this](Employee emp)
                               { return emp.employeeId == this->selectedAttendanceLog.employeeId; });

        if (it != allEmployees.end())
        {
            ui->empComboBox->setCurrentIndex(static_cast<int>(std::distance(allEmployees.begin(), it)));
        }
    }

    QDate date(selectedAttendanceLog.logDate.year,
               selectedAttendanceLog.logDate.month,
               selectedAttendanceLog.logDate.day);
    ui->DateEdit->setDate(date);

    QTime scheduledInTime = QTime::fromString(QString::fromStdString(selectedEmployee.clockInTimeStr), "hh:mm");
    QTime scheduledOutTime = QTime::fromString(QString::fromStdString(selectedEmployee.clockOutTimeStr), "hh:mm");

    int lateByMinute = selectedAttendanceLog.lateByMinute;
    int underTimeByMinute = selectedAttendanceLog.underTimeByMinute;

    scheduledInTime = scheduledInTime.addSecs(lateByMinute * 60);

    scheduledOutTime = scheduledOutTime.addSecs(1 - (underTimeByMinute * 60));

    ui->absentCheckBox->setChecked(selectedAttendanceLog.isAbsent);

    ui->notesTextEdit->setPlainText(QString::fromStdString(selectedAttendanceLog.notes));

    this->jsonDataFromDialog = selectedAttendanceLog.overtimeJson;

    Overtime ot = Overtime::fromJson(selectedAttendanceLog.overtimeJson);
    double minutes = ot.sumMinutes();
    ui->hourLabel->setText(QString::number(minutes) + " Minutes");

    scheduledOutTime = scheduledOutTime.addSecs(static_cast<int>(minutes * 60.0));

    ui->undertimeTimeEdit->setTime(scheduledOutTime);
    ui->latenessTimeEdit->setTime(scheduledInTime);
}

void AddTimeLogDialog::onEmployeeSelected()
{

    int index = ui->empComboBox->currentIndex();
    std::optional<Employee> emp = AppContext::instance().employeeService().getEmployeeByID(ui->empComboBox->itemData(index).toString().toStdString());
    if (emp.has_value())
    {
        this->selectedEmployee = emp.value();
    }
    else
    {
        this->selectedEmployee = {};
    }
    this->selectedAttendanceLog.employeeId = selectedEmployee.employeeId;
}

void AddTimeLogDialog::getFormData()
{
    QDate date = ui->DateEdit->date();

    QTime scheduledInTime = QTime::fromString(QString::fromStdString(selectedEmployee.clockInTimeStr), "hh:mm");
    QTime scheduledOutTime = QTime::fromString(QString::fromStdString(selectedEmployee.clockOutTimeStr), "hh:mm");

    QTime actualInTime = ui->latenessTimeEdit->time();
    QTime actualOutTime = ui->undertimeTimeEdit->time();

    int lateByMinute = scheduledInTime.secsTo(actualInTime) / 60;
    if (lateByMinute < 0)
        lateByMinute = 0;

    int underTimeByMinute = actualOutTime.secsTo(scheduledOutTime) / 60;
    if (underTimeByMinute < 0)
        underTimeByMinute = 0;

    int overTimeByMinute = scheduledOutTime.secsTo(actualOutTime) / 60;
    if (overTimeByMinute < 0)
        overTimeByMinute = 0;

    bool absent = ui->absentCheckBox->isChecked();
    QString notes = ui->notesTextEdit->toPlainText();

    this->selectedAttendanceLog.logDate = Date::fromString(date.toString("yyyy-MM-dd").toStdString());
    this->selectedAttendanceLog.lateByMinute = lateByMinute;
    this->selectedAttendanceLog.underTimeByMinute = underTimeByMinute;
    this->selectedAttendanceLog.overtimeJson = this->jsonDataFromDialog;
    this->selectedAttendanceLog.overTimeByMinute = Overtime::fromJson(this->jsonDataFromDialog).sumMinutes();
    this->selectedAttendanceLog.isAbsent = absent;
    this->selectedAttendanceLog.notes = notes.toStdString();
}
void AddTimeLogDialog::onSaveClicked()
{
    getFormData();
    if (AppContext::instance().attendanceLogService().updateAttendanceLog(this->selectedAttendanceLog))
    {
        LOG_DEBUG("Object Updated!");
        accept();
    }
    else
    {
        LOG_DEBUG("Updated Failed");
        reject();
    }
}
void AddTimeLogDialog::onOkayClicked()
{

    getFormData();
    int newId = AppContext::instance().attendanceLogService().addAttendanceLog(this->selectedAttendanceLog);
    if (newId > 0)
    {
        this->selectedAttendanceLog.logId = newId;
        LOG_DEBUG("Object added!: " << newId);
        accept();
    }
    else
    {
        LOG_DEBUG("add Failed");
        reject();
    }
};

AttendanceLog AddTimeLogDialog::getTimeLogData()
{
    return this->selectedAttendanceLog;
}

void AddTimeLogDialog::onCancelClicked()
{
    reject();
};

void AddTimeLogDialog::onSelectTypeClicked()
{

    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("Add_Overtime_Hours");
    auto overtimeHours = dynamic_cast<AddOvertimeHoursDialog *>(dlg);
    if (overtimeHours && overtimeHours->exec() == QDialog::Accepted)
    {
        std::string jsonString = overtimeHours->getJsonString();
        this->jsonDataFromDialog = jsonString;
        int otInMinutes = Overtime::fromJson(jsonString).sumMinutes();
        this->ui->hourLabel->setText(QString::fromStdString(std::to_string(otInMinutes) + " Minutes"));
    }
}

void AddTimeLogDialog::setEmployeeList(std::vector<Employee> &emps)
{
    this->allEmployees = emps;

    ui->empComboBox->clear();
    QStringList names;
    for (const auto &emp : allEmployees)
    {
        // Qstring to display                 //Data associated with the selection
        ui->empComboBox->addItem(QString::fromStdString(emp.fullName), QString::fromStdString(emp.employeeId));
    }
}

void AddTimeLogDialog::changeMode(std::string mode)
{

    auto okButton = ui->buttonBox->button(QDialogButtonBox::Ok);
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, nullptr);

    if (mode == "edit")
    {
        this->ui->empComboBox->setEnabled(false);
        okButton->setText("Save");
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddTimeLogDialog::onSaveClicked);
    }
    else if (mode == "add")
    {
        this->ui->empComboBox->setEnabled(true);
        okButton->setText("OK");
        connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddTimeLogDialog::onOkayClicked);
    }
}
