#include "Repositories/GovernmentRemittanceRepository.h"

GovernmentRemittanceRepository::GovernmentRemittanceRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\n GovernmentRemittanceRepository created" << std::endl;
}

GovernmentRemittance GovernmentRemittanceRepository::mapRemittance(sqlite3_stmt *stmt)
{
    GovernmentRemittance gr;
    int idx{0};

    gr.id = sqlite3_column_int(stmt, idx++);
    gr.payrollCalculationResultsId = sqlite3_column_int(stmt, idx++);
    gr.employeeId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++));
    gr.fullName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++));
    gr.employeeDepartment = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++));
    gr.payPeriodText = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++));
    gr.payPeriodHalf = sqlite3_column_int(stmt, idx++);
    gr.sssPremium_EE = sqlite3_column_double(stmt, idx++);
    gr.sssPremium_ER = sqlite3_column_double(stmt, idx++);
    gr.sssPremiumTotal = sqlite3_column_double(stmt, idx++);
    gr.sssSubmissionStatus = static_cast<RemittanceStatus>(sqlite3_column_int(stmt, idx++));
    gr.philHealthPremium_EE = sqlite3_column_double(stmt, idx++);
    gr.philHealthPremium_ER = sqlite3_column_double(stmt, idx++);
    gr.philHealthPremiumTotal = sqlite3_column_double(stmt, idx++);
    gr.phicSubmissionStatus = static_cast<RemittanceStatus>(sqlite3_column_int(stmt, idx++));
    gr.hdmfPremium_EE = sqlite3_column_double(stmt, idx++);
    gr.hdmfPremium_ER = sqlite3_column_double(stmt, idx++);
    gr.hdmfPremiumTotal = sqlite3_column_double(stmt, idx++);
    gr.hdmfSubmissionStatus = static_cast<RemittanceStatus>(sqlite3_column_int(stmt, idx++));
    gr.withHoldingTax = sqlite3_column_double(stmt, idx++);
    gr.withHoldingTaxSubmissionStatus = static_cast<RemittanceStatus>(sqlite3_column_int(stmt, idx++));
    gr.lastSubmittedDate = Date::fromString(reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++)));
    gr.submittedByUserId = sqlite3_column_int(stmt, idx++);
    gr.dateCreated = Date::fromString(reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++)));
    gr.dateModified = Date::fromString(reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++)));
    return gr;
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
    if (execute(sql, [&r](sqlite3_stmt *stmt)
                { bindRemittance(stmt, r); }))
        return sqlite3_last_insert_rowid(db);
    else
        return 0;
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
    std::string sql = std::format("select * from government_remittances where id = {};", id);
    auto results = BaseRepository::query<GovernmentRemittance>(sql, mapRemittance);
    if (results.size() > 0)
    {
        return results.front();
    }
    else
        return std::nullopt;
}
std::optional<GovernmentRemittance> GovernmentRemittanceRepository::getRemittanceByPayrollId(int payrollId)
{
    std::string sql = std::format("select * from government_remittances where payroll_calculation_results_id = {};", payrollId);
    auto results = BaseRepository::query<GovernmentRemittance>(sql, mapRemittance);
    if (results.size() > 0)
    {
        return results.front();
    }
    else
        return std::nullopt;
};
std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByEmployee(const std::string &employeeId)
{
    std::string sql = std::format("select * from government_remittances where employee_id = '{}';", employeeId);
    auto results = BaseRepository::query<GovernmentRemittance>(sql, mapRemittance);
    if (results.size() > 0)
    {
        return results;
    }
    else
        return {};
};
std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByStatus(RemittanceStatus status)
{
    std::string sql = std::format("select * from government_remittances WHERE sss_submission_status = {} OR phic_submission_status = {} OR hdmf_submission_status = {} OR withholding_tax_submission_status = {};", static_cast<int>(status), static_cast<int>(status), static_cast<int>(status), static_cast<int>(status));

    auto results = BaseRepository::query<GovernmentRemittance>(sql, mapRemittance);
    if (results.size() > 0)
    {
        return results;
    }
    else
        return {};
};

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByPeriod(const std::string &payPeriodText, std::optional<int> payPeriodHalf)
{
    std::string sql = "SELECT * FROM government_remittances WHERE pay_period_text = ?";
    if (payPeriodHalf.has_value())
        sql += " AND pay_period_half = ?";

    sqlite3_stmt *stmt = nullptr;
    std::vector<GovernmentRemittance> results{};

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        LOG_DEBUG("Failed to prepare: " << sqlite3_errmsg(db));
        return results;
    }
    int idx = 1;
    sqlite3_bind_text(stmt, idx++, payPeriodText.c_str(), -1, SQLITE_TRANSIENT);

    if (payPeriodHalf.has_value())
    {
        sqlite3_bind_int(stmt, idx++, payPeriodHalf.value());
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto obj = mapRemittance(stmt);
        results.push_back(obj);
    }

    sqlite3_finalize(stmt);
    return results;
}

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getAllRemittances()
{
    std::string sql = std::format("select * from government_remittances");
    auto results = BaseRepository::query<GovernmentRemittance>(sql, mapRemittance);
    if (results.size() > 0)
    {
        return results;
    }
    else
        return {};
}
GovernmentRemittanceRepository::MonthlySummary GovernmentRemittanceRepository::getMonthlySummary(const std::string &monthYear)
{
    // get all then loop through and add to the monthly summary object, then return the object
    auto items = GovernmentRemittanceRepository::getRemittancesByPeriod(monthYear);
    GovernmentRemittanceRepository::MonthlySummary summary{};
    summary.monthYear = monthYear;
    for (GovernmentRemittance &item : items)
    {
        summary.totalSSSEE += item.sssPremium_EE;
        summary.totalSSSER += item.sssPremium_ER;
        summary.totalPHICEE += item.philHealthPremium_EE;
        summary.totalPHICER += item.philHealthPremium_ER;
        summary.totalHDMFEE += item.hdmfPremium_EE;
        summary.totalHDMFER += item.hdmfPremium_ER;
        summary.totalWithholdingTax += item.withHoldingTax;
    }
    return summary;
};

// update -
bool GovernmentRemittanceRepository::markAsSubmitted(int remittanceId, const RemittanceType &type)
{
    std::string column;
    if (type == RemittanceType::SSS)
        column = " sss_submission_status";
    else if (type == RemittanceType::PHIC)
        column = " phic_submission_status";
    else if (type == RemittanceType::HDMF)
        column = " hdmf_submission_status";
    else
        return 0;
    std::string sql =
        "UPDATE government_remittances SET "
        "date_modified = ?, "
        "last_submitted_date = ?, " +
        column +
        " = ? WHERE id = ?";
    return execute(sql, [&remittanceId](sqlite3_stmt *stmt)
                   { 
                    int idx = 1; 
                    sqlite3_bind_text(stmt, idx++, Date::getTodayDate().to_string().c_str(), -1, SQLITE_TRANSIENT); 
                    sqlite3_bind_text(stmt, idx++, Date::getTodayDate().to_string().c_str(), -1, SQLITE_TRANSIENT); 
                    sqlite3_bind_int(stmt, idx++, static_cast<int>(RemittanceStatus::SUBMITTED)); 
                    sqlite3_bind_int(stmt, idx++, remittanceId); });
}

bool GovernmentRemittanceRepository::markAsConfirmed(int remittanceId, const RemittanceType &type)
{
    std::string column;
    if (type == RemittanceType::SSS)
        column = " sss_submission_status";
    else if (type == RemittanceType::PHIC)
        column = " phic_submission_status";
    else if (type == RemittanceType::HDMF)
        column = " hdmf_submission_status";
    else
        return 0;
    std::string sql =
        "UPDATE government_remittances SET "
        "date_modified = ?, " +
        column +
        " = ? WHERE id = ?";
    return execute(sql, [&remittanceId](sqlite3_stmt *stmt)
                   { 
                    int idx = 1; 
                    sqlite3_bind_text(stmt, idx++, Date::getTodayDate().to_string().c_str(), -1, SQLITE_TRANSIENT); 
                    sqlite3_bind_int(stmt, idx++, static_cast<int>(RemittanceStatus::CONFIRMED)); 
                    sqlite3_bind_int(stmt, idx++, remittanceId); });
}

bool GovernmentRemittanceRepository::markAsRejected(int remittanceId, const RemittanceType &type)
{
    std::string column;
    if (type == RemittanceType::SSS)
        column = " sss_submission_status";
    else if (type == RemittanceType::PHIC)
        column = " phic_submission_status";
    else if (type == RemittanceType::HDMF)
        column = " hdmf_submission_status";
    else
        return 0;
    std::string sql =
        "UPDATE government_remittances SET "
        "date_modified = ?, " +
        column +
        " = ? WHERE id = ?";
    return execute(sql, [&remittanceId](sqlite3_stmt *stmt)
                   { 
                    int idx = 1; 
                    sqlite3_bind_text(stmt, idx++, Date::getTodayDate().to_string().c_str(), -1, SQLITE_TRANSIENT); 
                    sqlite3_bind_int(stmt, idx++, static_cast<int>(RemittanceStatus::REJECTED)); 
                    sqlite3_bind_int(stmt, idx++, remittanceId); });
}

bool GovernmentRemittanceRepository::updateRemittance(const GovernmentRemittance &remittance)
{
    std::string sql = R"(update government_remittances set  
        payroll_calculation_results_id = ?,
                employee_id =?,
                full_name =?,
                employee_department = ?,
                pay_period_text = ?,
                pay_period_half = ?,
                sss_premium_ee = ?,
                sss_premium_er = ?,
                sss_premium_total = ?,
                sss_submission_status = ?,
                phic_premium_ee = ?,
                phic_premium_er = ?,
                phic_premium_total = ?,
                phic_submission_status = ?,
                hdmf_premium_ee = ?,
                hdmf_premium_er = ?,
                hdmf_premium_total = ?,
                hdmf_submission_status = ?,
                withholding_tax = ?,
                withholding_tax_submission_status = ?,
                last_submitted_date = ?,
                submitted_by_user_id = ?,
                date_created = ?,
                date_modified = ?
                where id = ? )";
    return execute(sql, [&remittance](sqlite3_stmt *stmt)
                   { 
                    int idx = bindRemittance(stmt, remittance); 
                    sqlite3_bind_int(stmt, idx, remittance.id); });
}

bool GovernmentRemittanceRepository::deleteRemittance(int id)
{
    std::string sql = R"(
    delete 
    from government_remittances 
    where id = ?
    )";
    return execute(sql, [&id](sqlite3_stmt *stmt)
                   { sqlite3_bind_int(stmt, 1, id); });
}

int GovernmentRemittanceRepository::bindRemittance(sqlite3_stmt *stmt, const GovernmentRemittance &r)
{
    int idx{1};
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
    return idx;
}