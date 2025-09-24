#include "../../include/modules/AdminSettingsModule.hpp"
#include <iostream>

// AdminSettingsModule
AdminSettingsModule::AdminSettingsModule() {
    std::cout << "AdminSettingsModule created\n";
}
AdminSettingsModule::AdminSettingsModule(ActionDispatcher& dispatcher) {
    std::cout << "AdminSettingsModule created\n";
    dispatcher.regAction("admin_settings", "manage_login_security", std::bind(&AdminSettingsModule::configureLoginSecurity , this));
}
AdminSettingsModule::~AdminSettingsModule() {
    std::cout << "AdminSettingsModule destroyed\n";
}
void AdminSettingsModule::configureLoginSecurity()
{
    std::cout << "AdminSettingsModule::configureLoginSecurity()" << std::endl;
};

// UserRoleManager
UserRoleManager::UserRoleManager() {
    std::cout << "UserRoleManager created\n";
}
UserRoleManager::UserRoleManager(ActionDispatcher& dispatcher) {
    std::cout << "UserRoleManager created\n";
    dispatcher.regAction("manage_roles", "role_admin",  std::bind(&UserRoleManager::configureAdminRole , this));
    dispatcher.regAction("manage_roles", "role_hr", std::bind(&UserRoleManager::configureHRStaffRole , this));

}
UserRoleManager::~UserRoleManager() {
    std::cout << "UserRoleManager destroyed\n";
}
void UserRoleManager::configureAdminRole()
{
    std::cout << "UserRoleManager::configureAdminRole()" << std::endl;
};
void UserRoleManager::configureHRStaffRole()
{
    std::cout << "UserRoleManager::configureHRStaffRole()" << std::endl;
};

// ContributionTable
ContributionTable::ContributionTable() {
    std::cout << "ContributionTable created\n";
}
ContributionTable::ContributionTable(ActionDispatcher& dispatcher) {
    std::cout << "ContributionTable created\n";
    dispatcher.regAction("contrib_tables", "edit_sss_rates", std::bind(&ContributionTable::editSSSRates , this));
    dispatcher.regAction("contrib_tables", "edit_phic_rates",std::bind(&ContributionTable::editPHICRates, this));
    dispatcher.regAction("contrib_tables", "edit_hdmf_rates",std::bind(&ContributionTable::editHDMFRates , this));
    dispatcher.regAction("contrib_tables", "edit_tax_table", std::bind(&ContributionTable::editWithholdingTaxTable, this));
}
ContributionTable::~ContributionTable() {
    std::cout << "ContributionTable destroyed\n";
}
void ContributionTable::editSSSRates()
{
    std::cout << "ContributionTable::editSSSRates()" << std::endl;
};
void ContributionTable::editPHICRates()
{
    std::cout << "ContributionTable::editPHICRates()" << std::endl;
};
void ContributionTable::editHDMFRates()
{
    std::cout << "ContributionTable::editHDMFRates()" << std::endl;
};
void ContributionTable::editWithholdingTaxTable()
{
    std::cout << "ContributionTable::editWithholdingTaxTable()" << std::endl;
};
