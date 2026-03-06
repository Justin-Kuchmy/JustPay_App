#include "Repositories/GovernmentRemittanceRepository.h"

GovernmentRemittanceRepository::GovernmentRemittanceRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\n GovernmentRemittanceRepository created" << std::endl;
}

GovernmentRemittance GovernmentRemittanceRepository::mapRemittance(sqlite3_stmt *stmt)
{
    return {};
}

bool GovernmentRemittanceRepository::createTable() const
{
    return BaseRepository::executeFile(":/resources/sql/governmentremittance.sql");
}

// create
sqlite3_int64 GovernmentRemittanceRepository::insertRemittance(const GovernmentRemittance &r)
{
    return {};
}

std::optional<GovernmentRemittance> GovernmentRemittanceRepository::getRemittanceById(int id)
{
    return std::nullopt;
}

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByPeriod(const std::string &payPeriodText, int payPeriodHalf)
{
    return {};
}

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getAllRemittances()
{
    return {};
}

// update -
bool GovernmentRemittanceRepository::markAsSubmitted(int remittanceId, const std::string &remittanceType, int submittedByUserId, const Date &submissionDate)
{
    return 0;
}

bool GovernmentRemittanceRepository::markAsConfirmed(int remittanceId, const std::string &remittanceType)
{
    return 0;
}

bool GovernmentRemittanceRepository::markAsRejected(int remittanceId, const std::string &remittanceType)
{
    return 0;
}

bool GovernmentRemittanceRepository::updateRemittance(const GovernmentRemittance &remittance)
{
    return 0;
}

bool GovernmentRemittanceRepository::deleteRemittance(int id)
{
    return 0;
}

GovernmentRemittanceRepository::MonthlySummary GovernmentRemittanceRepository::getMonthlySummary(const std::string &monthYear)
{
    return {};
};
