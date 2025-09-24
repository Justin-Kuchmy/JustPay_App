#pragma once
#include <iostream>

class AdminSettingsModule {
public:
    AdminSettingsModule();
    ~AdminSettingsModule();
    void configureLoginSecurity();
};

class UserRoleManager {
public:
    UserRoleManager();
    ~UserRoleManager();
    void configureAdminRole();
    void configureHRStaffRole();
};

class ContributionTable {
public:
    ContributionTable();
    ~ContributionTable();
    void editSSSRates();
    void editPHICRates();
    void editHDMFRates();
    void editWithholdingTaxTable();
};
