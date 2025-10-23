
#include "Utils/DialogFactory.h"
#include "UI/Add_Employee_Dialog.h"
#include "UI/Add_Dependent_Dialog.h"
#include "UI/Add_Emergency_Contact_Dialog.h"


std::unordered_map<QString, DialogFactory::Creator> DialogFactory::registry;

void DialogFactory::registerDialogs()
{
    registry.clear();
    registry["add_employee"] = []() { return new AddEmployeeDialog(); };
    registry["add_contact"] = []() { return new EmergencyContactDialog(); };
    registry["add_dependent"] = []() { return new AddDependentDialog(); };
    
};

QDialog* DialogFactory::create(const QString& key) {
    if (registry.contains(key))
        return registry[key]();
    return nullptr;
}
