
#include "include/Generated/ui_employee_attendance_widget.h"
#include "UI/Employee_Attendance_Widget.h"
#include "Services/AppContext.h"

EmployeeAttendanceWidget::EmployeeAttendanceWidget(QWidget *parent): QWidget(parent), ui(new Ui::EmployeeAttendanceWidget),
attendanceLogs{},model{},proxyModel{},employeeId{}
{
    ui->setupUi(this); 
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(ui->attendanceLogTableView);
    setLayout(mainLayout);
  
    
}

EmployeeAttendanceWidget::~EmployeeAttendanceWidget()
{
    delete ui;
};

void EmployeeAttendanceWidget::setEmployeeAttendanceContext(std::vector<AttendanceLog> *vectorPtr, std::string& employeeId)
{

    if (!vectorPtr) 
        return;
    this->attendanceLogs = vectorPtr;

    model = new AttendanceLogModel(this, *attendanceLogs);

    //model wrapper to allow table filtering
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    //column 0 is uses for the filter, in this case we filter by employee id
    proxyModel->setFilterKeyColumn(0);

    // set model on the table view
    ui->attendanceLogTableView->setModel(proxyModel);

    //stretch table width to match window
    if (ui->attendanceLogTableView->horizontalHeader()) 
    {
        ui->attendanceLogTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }
    //click on a column to sort
    ui->attendanceLogTableView->setSortingEnabled(true);
    this->employeeId = employeeId;

    //this->populateLoanList();
    //this->setDefaults();

}