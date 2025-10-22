
#include "Utils/DialogFactory.h"
#include "UI/Add_Employee_Dialog.h"


std::unordered_map<QString, DialogFactory::Creator> DialogFactory::registry;

void DialogFactory::registerDialogs()
{
    registry.clear();
    registry["add_employee"] = []() { return new AddEmployeeDialog(); };
    //registry.insert("add_employee", []() { return new AddEmployeeDialog(); });
    
};

QDialog* DialogFactory::create(const QString& key) {
    if (registry.contains(key))
        return registry[key]();
    return nullptr;
}
