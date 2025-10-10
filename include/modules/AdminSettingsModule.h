#pragma once
#include <iostream>
#include "../ActionDispatcher.h"

class AdminSettingsModule {
public:
    AdminSettingsModule();
    AdminSettingsModule(ActionDispatcher& dispatcher);
    ~AdminSettingsModule();
    void configureLoginSecurity();
};

class UserRoleManager {
public:
    UserRoleManager();
    UserRoleManager(ActionDispatcher& dispatcher);
    ~UserRoleManager();
    void configureAdminRole();
    void configureHRStaffRole();
};

class ContributionTable {
public:
    ContributionTable();
    ContributionTable(ActionDispatcher& dispatcher);
    ~ContributionTable();
    void editSSSRates();
    void editPHICRates();
    void editHDMFRates();
    void editWithholdingTaxTable();
};
