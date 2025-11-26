#include "include/Generated/ui_attendance_list_widget.h"
#include "UI/Attendance_List_Widget.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"


AttendanceListWidget::AttendanceListWidget(QWidget *parent): QWidget(parent), ui(new Ui::AttendanceListWidget)
{
    ui->setupUi(this);
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

};
