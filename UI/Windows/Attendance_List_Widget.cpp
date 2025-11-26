#include "include/Generated/ui_attendance_list_widget.h"
#include "UI/Attendance_List_Widget.h"
#include "UI/Add_Time_Log_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"


AttendanceListWidget::AttendanceListWidget(QWidget *parent): QWidget(parent), ui(new Ui::AttendanceListWidget)
{
    ui->setupUi(this);
    connect(ui->btnAddEntry,&QPushButton::clicked,this,&AttendanceListWidget::onAddClicked);
};

AttendanceListWidget::~AttendanceListWidget()
{
    delete ui;
};

void AttendanceListWidget::onReloadClicked()
{

};

void AttendanceListWidget::onAddClicked()
{
    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("Add_Time_Log");
    auto timeLog = dynamic_cast<AddTimeLogDialog*>(dlg);
    if(timeLog && timeLog->exec() == QDialog::Accepted)
    {

    }

};
