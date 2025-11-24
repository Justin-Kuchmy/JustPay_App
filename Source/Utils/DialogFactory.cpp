
#include "Utils/DialogFactory.h"
#include "UI/Add_Employee_Dialog.h"
#include "UI/Add_Dependent_Dialog.h"
#include "UI/Add_Emergency_Contact_Dialog.h"
#include "UI/Employee_Details_Widget.h"
#include "UI/Add_Loan_Ledger_dialog.h"
#include "UI/SqlQueryWidget.h"
#include "UI/Add_Sql_Description.h"




std::unordered_map<QString, DialogFactory::Creator> DialogFactory::registry;


void DialogFactory::registerDialogs()
{
    registry.clear();
    registry["add_employee"] = []() { return new AddEmployeeDialog(); };
    registry["add_contact"] = []() { return new EmergencyContactDialog(); };
    registry["add_dependent"] = []() { return new AddDependentDialog(); };
    registry["select_employee"] = []() { return new EmployeeDetailsWidget(); };
    registry["add_loan_ledger"] = []() { return new AddLoanLedgerDialog(); };
    registry["sql_query_module"] = []() { return new SqlQueryWidget(); };
    registry["add_sql_description"] = []() { return new AddSqlDescriptionDialog(); };
    

    
};

QWidget* DialogFactory::create(const QString& key) {
    if (registry.contains(key))
        return registry[key]();
    return nullptr;
}
