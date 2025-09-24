#include "../../include/modules/AdminSettingsModule.hpp"
#include <iostream>

// AdminSettingsModule
AdminSettingsModule::AdminSettingsModule() {
    std::cout << "AdminSettingsModule created\n";
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
