

#include "include/Generated/ui_add_time_log_dialog.h"
#include "UI/Add_Time_Log_Dialog.h"
#include "UI/Add_Overtime_Hours_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"
#include "Services/AppContext.h"
#include <iterator>

AddTimeLogDialog::AddTimeLogDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddTimeLogDialog)
{
    ui->setupUi(this);
    connect(ui->selectOvertimeType,&QPushButton::clicked,this,&AddTimeLogDialog::onSelectTypeClicked);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddTimeLogDialog::onOkayClicked); //to close myself
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

    //if log successfully taken from the database. 
    if(optLog.has_value())
    {
        //get object
        selectedAttendanceLog = optLog.value();

        //search for the log with the matching employeeID that we selected
        auto it = std::find_if(allEmployees.begin(), allEmployees.end(), [this](Employee emp) {return emp.employeeId == this->selectedAttendanceLog.employeeId;});
        //if we found it
        if (it != allEmployees.end()) {
            //the distance between the start and the found item, thus gives us the index
        std::size_t index = std::distance(allEmployees.begin(), it);
            //sets combobox to the same index. 
            ui->empComboBox->setCurrentIndex(index);
        }
    }
    
    QDate date(selectedAttendanceLog.logDate.year,
               selectedAttendanceLog.logDate.month,
               selectedAttendanceLog.logDate.day);
    ui->DateEdit->setDate(date);

    // Set the spinboxes
    ui->latespinBox->setValue(selectedAttendanceLog.lateByMinute);
    ui->undertimeSpinbox->setValue(selectedAttendanceLog.underTimeByMinute);

    // Set the checkbox
    ui->absentCheckBox->setChecked(selectedAttendanceLog.isAbsent);

    // Set the notes
    ui->notesTextEdit->setPlainText(QString::fromStdString(selectedAttendanceLog.notes));

    // If you also have overtime JSON or other fields, store it internally
    this->jsonDataFromDialog = selectedAttendanceLog.overtimeJson;

    // Optionally, update UI elements that depend on overtimeJson
    Overtime ot = Overtime::fromJson(selectedAttendanceLog.overtimeJson);
    double minutes = ot.sumMinutes();
    ui->hourLabel->setText(QString::number(minutes) + " Minutes");
}


void AddTimeLogDialog::onEmployeeSelected()
{
    // when user clicks OK
    int index = ui->empComboBox->currentIndex();
    this->al.employeeId = ui->empComboBox->itemData(index).toString().toStdString();
}

void AddTimeLogDialog::onOkayClicked()
{
    QDate date = ui->DateEdit->date();
    int latespinBox = ui->latespinBox->value();
    int undertimeSpinbox = ui->undertimeSpinbox->value();
    bool absent = ui->absentCheckBox->isChecked();
    QString notes = ui->notesTextEdit->toPlainText();
    
    this->al.logId = 0;
    this->al.logDate = Date::fromString(date.toString("yyyy-MM-dd").toStdString());
    this->al.lateByMinute = latespinBox;
    this->al.underTimeByMinute = undertimeSpinbox;
    this->al.overTimeByMinute = 0;
    this->al.overtimeJson = this->jsonDataFromDialog;
    this->al.isAbsent = absent;
    this->al.notes = notes.toStdString();

    int newId = AppContext::instance().attendanceLogService().addAttendanceLog(this->al);
    if(newId > 0)
    {
        this->al.logId = newId;
        accept();
    }
    else
    {
        reject();
    }
    
};

AttendanceLog AddTimeLogDialog::getTimeLogData()
{
    return this->al;
}

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
    for (const auto& emp : allEmployees)
    {
                                 //Qstring to display                 //Data associated with the selection
        ui->empComboBox->addItem(QString::fromStdString(emp.fullName), QString::fromStdString(emp.employeeId));    
    }

}

void AddTimeLogDialog::changeMode(std::string mode)
{
    if (mode == "edit") 
    {
        this->ui->empComboBox->setEnabled(false);
    } 
    else if (mode == "add") 
    {
        this->ui->empComboBox->setEnabled(true);
    }
}



    
