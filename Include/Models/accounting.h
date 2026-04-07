#pragma once
#include "Models/Primitives/enums.h"

struct JournalEntry
{
    int entryId{0};
    AccountType accountType{};
    std::string accountName{""};
    double debit{0.0};
    double credit{0.0};
    std::string periodText{""};
    std::string periodHalf{""};
};

struct EmailCrudentials
{
    std::string companyEmail{};
    std::string appPassword{};

    std::string to_string() const
    {
        return {"companyEmail " + companyEmail + "\nappPassword: " + appPassword};
    }
};
