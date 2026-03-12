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
    const char *sql = R"(INSERT OR IGNORE INTO government_remittances (
            payroll_calculation_results_id,
            employee_id,
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

    int idx = 1;
    sqlite3_bind_int(stmt, idx++, r.payrollCalculationResultsId);
    sqlite3_bind_text(stmt, idx++, r.employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, r.fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, r.employeeDepartment.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, r.payPeriodText.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, r.payPeriodHalf);

    sqlite3_bind_double(stmt, idx++, r.sssPremium_EE);
    sqlite3_bind_double(stmt, idx++, r.sssPremium_ER);
    sqlite3_bind_double(stmt, idx++, r.sssPremiumTotal);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.sssSubmissionStatus));

    sqlite3_bind_double(stmt, idx++, r.philHealthPremium_EE);
    sqlite3_bind_double(stmt, idx++, r.philHealthPremium_ER);
    sqlite3_bind_double(stmt, idx++, r.philHealthPremiumTotal);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.phicSubmissionStatus));

    sqlite3_bind_double(stmt, idx++, r.hdmfPremium_EE);
    sqlite3_bind_double(stmt, idx++, r.hdmfPremium_ER);
    sqlite3_bind_double(stmt, idx++, r.hdmfPremiumTotal);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.hdmfSubmissionStatus));

    sqlite3_bind_double(stmt, idx++, r.withHoldingTax);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.withHoldingTaxSubmissionStatus));

    sqlite3_bind_text(stmt, idx++, r.lastSubmittedDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, r.submittedByUserId);
    sqlite3_bind_text(stmt, idx++, r.dateCreated.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, r.dateModified.to_string().c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE)
    {
        LOG_DEBUG("SQL insert failed: " << sqlite3_errmsg(db));
        return 0;
    }

    if (sqlite3_changes(db) == 0)
    {
        LOG_DEBUG("Duplicate remittance record, insert skipped");
        return 0;
    }

    return sqlite3_last_insert_rowid(db);
}
std::vector<sqlite3_int64> GovernmentRemittanceRepository::insertRemittanceReports(const std::vector<GovernmentRemittance> &r)
{
    std::vector<sqlite3_int64> ids;

    sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

    for (const auto &remittance : r)
    {
        auto id = insertRemittance(remittance);
        if (id > 0)
            ids.push_back(id);
        else
        {
            // rollback everything if any insert fails
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
            return {};
        }
    }
    sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
    return ids;
}

std::optional<GovernmentRemittance> GovernmentRemittanceRepository::getRemittanceById(int id)
{
    return std::nullopt;
}
std::optional<GovernmentRemittance> GovernmentRemittanceRepository::getRemittanceByPayrollId(int payrollId)
{
    return std::nullopt;
};
std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByEmployee(const std::string &employeeId)
{
    return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByStatus(RemittanceStatus status)
{
    return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByPeriod(const std::string &payPeriodText, std::optional<int> payPeriodHalf)
{
    return {};
}

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getAllRemittances()
{
    return {};
}
GovernmentRemittanceRepository::MonthlySummary GovernmentRemittanceRepository::getMonthlySummary(const std::string &monthYear)
{
    return {};
};

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
