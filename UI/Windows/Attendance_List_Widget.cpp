#include "include/Generated/ui_attendance_list_widget.h"
#include "UI/Attendance_List_Widget.h"
#include "UI/Add_Time_Log_Dialog.h"
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Utils/DialogFactory.h"
#include "Services/AppContext.h"


AttendanceListWidget::AttendanceListWidget(QWidget *parent): BaseContentWidget(parent), ui(new Ui::AttendanceListWidget)
{
    ui->setupUi(this);
    connect(ui->btnAddEntry,&QPushButton::clicked,this,&AttendanceListWidget::onAddClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &BaseContentWidget::backRequested);
    connect(ui->employeeSearchBox, &QLineEdit::textChanged,this, &AttendanceListWidget::onEmployeeSearchBoxChanged);
    connect(ui->attendanceLogTableView, &QTableView::clicked,this, &AttendanceListWidget::onRowClicked);

    //get logs from database
    attendanceLogs = std::vector<AttendanceLog>(AppContext::instance().attendanceLogService().getAllAttendanceLogs());

    model = new AttendanceLogModel(this, attendanceLogs);

    //model wrapper to allow table filtering
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    //column 0 is uses for the filter, in this case we filter by employee id
    proxyModel->setFilterKeyColumn(0);


    // set model on the table view
    ui->attendanceLogTableView->setModel(proxyModel);

    //stretch table width to match window
    ui->attendanceLogTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //click on a column to sort
    ui->attendanceLogTableView->setSortingEnabled(true);

};

void AttendanceListWidget::onEmployeeSearchBoxChanged(const QString& filter)
{
    proxyModel->setFilterFixedString(filter);
}

void AttendanceListWidget::onRowClicked(const QModelIndex &index)
{
    std::vector<Employee> AllEmplyees = AppContext::instance().employeeService().getAllEmployees();

    //gets logId of the AttendanceLog you clicked on
    int logId = index.model()->data(index, Qt::UserRole).toInt();

    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("Add_Time_Log");
    AddTimeLogDialog* timeLog = dynamic_cast<AddTimeLogDialog*>(dlg);

    //disables Employee Select ComboBox
    timeLog->changeMode("edit");
    timeLog->setEmployeeList(AllEmplyees);

    //Pass in Selected LogID
    timeLog->getLogData(logId);
    
    if(timeLog && timeLog->exec() == QDialog::Accepted)
    {
        this->onReloadLogs();
    }

}


AttendanceListWidget::~AttendanceListWidget()
{
    delete ui;
};


void AttendanceListWidget::onReloadLogs()
{
    auto &ctx = AppContext::instance();
    auto &service = ctx.attendanceLogService();

    auto newLogs = service.getAllAttendanceLogs();
    attendanceLogs = std::move(newLogs);
    
    model->reloadData(attendanceLogs);
};

void AttendanceListWidget::onAddClicked()
{
    std::vector<Employee> AllEmplyees = AppContext::instance().employeeService().getAllEmployees();

    DialogFactory::registerDialogs();
    auto dlg = DialogFactory::create("Add_Time_Log");
    AddTimeLogDialog* timeLog = dynamic_cast<AddTimeLogDialog*>(dlg);
    timeLog->setEmployeeList(AllEmplyees);
    timeLog->changeMode("add");
    if(timeLog && timeLog->exec() == QDialog::Accepted)
    {
        this->onReloadLogs();
    }
};


