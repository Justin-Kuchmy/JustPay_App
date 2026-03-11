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

sqlite3_int64 GovernmentRemittanceRepository::insertRemittanceReports(const std::vector<GovernmentRemittance> &r)
{
    const char *sql = R"(INSERT INTO government_remittances (
        payroll_calculation_results_id,
        employee_Id,
        full_name,
        employee_department,
        pay_period_text,
        pay_period_half,
        sss_premium_ee,
        sss_premium_er,
        sss_premium_total,
        sss_submission_status,
        phic_premium_ee,
        phic_premium_er,
        phic_premium_total,
        phic_submission_status,
        hdmf_premium_ee,
        hdmf_premium_er,
        hdmf_premium_total,
        hdmf_submission_status,
        withholding_tax,
        withholding_tax_submission_status,
        last_submitted_date,
        submitted_by_user_id,
        date_created,
        date_modified
        ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)
    )";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("SQL prepare failed: " << sqlite3_errmsg(db));
        return 0;
    }

    for (const auto &object : r)
    {

        int idx = 1;
        sqlite3_bind_int(stmt, idx++, object.payrollCalculationResultsId);
        sqlite3_bind_text(stmt, idx++, object.employeeId.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, object.fullName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, object.employeeDepartment.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, object.payPeriodText.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, idx++, object.payPeriodHalf);

        sqlite3_bind_double(stmt, idx++, object.sssPremium_EE);
        sqlite3_bind_double(stmt, idx++, object.sssPremium_ER);
        sqlite3_bind_double(stmt, idx++, object.sssPremiumTotal);
        sqlite3_bind_int(stmt, idx++, static_cast<int>(object.sssSubmissionStatus));

        sqlite3_bind_double(stmt, idx++, object.philHealthPremium_EE);
        sqlite3_bind_double(stmt, idx++, object.philHealthPremium_ER);
        sqlite3_bind_double(stmt, idx++, object.philHealthPremiumTotal);
        sqlite3_bind_int(stmt, idx++, static_cast<int>(object.phicSubmissionStatus));

        sqlite3_bind_double(stmt, idx++, object.hdmfPremium_EE);
        sqlite3_bind_double(stmt, idx++, object.hdmfPremium_ER);
        sqlite3_bind_double(stmt, idx++, object.hdmfPremiumTotal);
        sqlite3_bind_int(stmt, idx++, static_cast<int>(object.hdmfSubmissionStatus));

        sqlite3_bind_double(stmt, idx++, object.withHoldingTax);
        sqlite3_bind_int(stmt, idx++, static_cast<int>(object.withHoldingTaxSubmissionStatus));

        sqlite3_bind_text(stmt, idx++, object.lastSubmittedDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, idx++, object.submittedByUserId);
        sqlite3_bind_text(stmt, idx++, object.dateCreated.to_string().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, object.dateModified.to_string().c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);

        if (rc != SQLITE_DONE)
        {
            LOG_DEBUG("SQL insert failed: " << sqlite3_errmsg(db));
            return 0;
        }

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }
    sqlite3_finalize(stmt);
    return sqlite3_last_insert_rowid(db);
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
