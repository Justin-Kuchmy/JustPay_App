
#include "Utils/DialogFactory.h"
#include "UI/Employee_Management/Add_Employee_Dialog.h"
#include "UI/Employee_Management/Add_Dependent_Dialog.h"
#include "UI/Employee_Management/Add_Emergency_Contact_Dialog.h"
#include "UI/Employee_Management/Employee_Details_Widget.h"
#include "UI/Employee_Management/Add_Loan_Ledger_dialog.h"
#include "UI/AdminSettings/SqlQueryWidget.h"
#include "UI/AdminSettings/Add_Sql_Description.h"
#include "UI/AdminSettings/Email_Login_Widget.h"
#include "UI/AttendanceTracking/Add_Time_Log_Dialog.h"
#include "UI/AttendanceTracking/Add_Overtime_Hours_Dialog.h"
#include "UI/AttendanceTracking/Attendance_List_Widget.h"
#include "UI/PayrollComputation/Payroll_Widget.h"
#include "UI/PayslipGeneration/Payslip_Generation_Widget.h"
#include "UI/Reports-Logs/Reports_Logs_Widget.h"

std::unordered_map<QString, DialogFactory::Creator> DialogFactory::registry;

void DialogFactory::registerDialogs()
{
    registry.clear();
    registry["add_employee"] = []()
    { return new AddEmployeeDialog(); };
    registry["add_contact"] = []()
    { return new EmergencyContactDialog(); };
    registry["add_dependent"] = []()
    { return new AddDependentDialog(); };
    registry["select_employee"] = []()
    { return new EmployeeDetailsWidget(); };
    registry["add_loan_ledger"] = []()
    { return new AddLoanLedgerDialog(); };
    registry["sql_query_module"] = []()
    { return new SqlQueryWidget(); };
    registry["add_sql_description"] = []()
    { return new AddSqlDescriptionDialog(); };
    registry["attendance_list"] = []()
    { return new AttendanceListWidget(); };
    registry["Add_Time_Log"] = []()
    { return new AddTimeLogDialog(); };
    registry["Add_Overtime_Hours"] = []()
    { return new AddOvertimeHoursDialog(); };
    registry["payroll-compute_widget"] = []()
    { return new PayrollWidget(); };
    registry["payslip-generation_menu"] = []()
    { return new PayslipWidget(); };
    registry["email_auth_module"] = []()
    { return new EmailLoginWidget(); };
    registry["reports-logs_menu"] = []()
    { return new ReportsLogsWidget(); };
};

QWidget *DialogFactory::create(const QString &key)
{
    if (registry.contains(key))
        return registry[key]();
    return nullptr;
}
