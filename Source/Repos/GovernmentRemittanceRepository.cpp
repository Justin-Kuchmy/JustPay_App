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
    gr.employeeDepartment = sqlite3_column_int(stmt, idx++);
    gr.payPeriodDate = reinterpret_cast<const char *>(sqlite3_column_text(stmt, idx++));
    gr.payPeriodHalf = sqlite3_column_int(stmt, idx++);
    gr.employee_Contrib = sqlite3_column_double(stmt, idx++);
    gr.employer_Contrib = sqlite3_column_double(stmt, idx++);
    gr.total_Contrib = sqlite3_column_double(stmt, idx++);
    gr.contrib_Type = static_cast<RemittanceType>(sqlite3_column_int(stmt, idx++));
    gr.submissionStatus = static_cast<RemittanceStatus>(sqlite3_column_int(stmt, idx++));

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
            pay_period_date,
            pay_period_half,
            employee_Contrib,
            employer_Contrib,
            total_Contrib,
            remittance_type,
            submission_status,
            withholding_tax,
            withholding_tax_submission_status,
            last_submitted_date,
            submitted_by_user_id,
            date_created,
            date_modified
        ) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)
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
    const char *sql = "select * from government_remittances where submission_status = ?";
    return query<GovernmentRemittance>(sql, mapRemittance, [status](sqlite3_stmt *stmt)
                                       { sqlite3_bind_int(stmt, 1, static_cast<int>(status)); });
};

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getRemittancesByPeriod(const std::string &payPeriodDate, std::optional<int> payPeriodHalf)
{

    std::string sql = "select * from government_remittances where pay_period_date = ?";
    if (payPeriodHalf.has_value())
        sql += " AND pay_period_half = ?";
    return query<GovernmentRemittance>(sql, mapRemittance, [payPeriodDate, payPeriodHalf](sqlite3_stmt *stmt)
                                       { 
                                        sqlite3_bind_text(stmt, 1, payPeriodDate.c_str(), -1, SQLITE_TRANSIENT);
                                        if (payPeriodHalf.has_value()) sqlite3_bind_int(stmt, 2, payPeriodHalf.value()); });
}

std::vector<GovernmentRemittance> GovernmentRemittanceRepository::getAllRemittances()
{
    const char *sql = "select * from government_remittances";
    return query<GovernmentRemittance>(sql, mapRemittance);
}

// update -
bool GovernmentRemittanceRepository::markAsSubmitted(int remittanceId, const RemittanceType &type)
{
    const char *sql = "UPDATE government_remittances SET date_modified = ?, last_submitted_date = ?, submission_status =? WHERE id = ? ";
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
    const char *sql = "UPDATE government_remittances SET date_modified = ?,  submission_status =? WHERE id = ? ";
    return execute(sql, [&remittanceId](sqlite3_stmt *stmt)
                   { 
                    int idx = 1; 
                    sqlite3_bind_text(stmt, idx++, Date::getTodayDate().to_string().c_str(), -1, SQLITE_TRANSIENT); 
                    sqlite3_bind_int(stmt, idx++, static_cast<int>(RemittanceStatus::CONFIRMED)); 
                    sqlite3_bind_int(stmt, idx++, remittanceId); });
}

bool GovernmentRemittanceRepository::markAsRejected(int remittanceId, const RemittanceType &type)
{

    const char *sql = "UPDATE government_remittances SET date_modified = ?, submission_status = ? WHERE id = ?";
    return execute(sql, [&remittanceId](sqlite3_stmt *stmt)
                   { 
                    int idx = 1; 
                    sqlite3_bind_text(stmt, idx++, Date::getTodayDate().to_string().c_str(), -1, SQLITE_TRANSIENT); 
                    sqlite3_bind_int(stmt, idx++, static_cast<int>(RemittanceStatus::REJECTED)); 
                    sqlite3_bind_int(stmt, idx++, remittanceId); });
}

bool GovernmentRemittanceRepository::updateRemittance(const GovernmentRemittance &remittance)
{
    const char *sql = R"(update government_remittances set  
        payroll_calculation_results_id = ?,
                employee_id =?,
                full_name =?,
                employee_department = ?,
                pay_period_date = ?,
                pay_period_half = ?,
                employee_Contrib = ?,
                employer_Contrib = ?,
                total_Contrib = ?,
                remittance_type = ?,
                submission_status = ?,
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
    const char *sql = "delete from government_remittances where id =? ";
    return execute(sql, [&id](sqlite3_stmt *stmt)
                   { sqlite3_bind_int(stmt, 1, id); });
}

int GovernmentRemittanceRepository::bindRemittance(sqlite3_stmt *stmt, const GovernmentRemittance &r)
{
    int idx{1};
    sqlite3_bind_int(stmt, idx++, r.payrollCalculationResultsId);
    sqlite3_bind_text(stmt, idx++, r.employeeId.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, r.fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, r.employeeDepartment);
    sqlite3_bind_text(stmt, idx++, r.payPeriodDate.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, r.payPeriodHalf);
    sqlite3_bind_double(stmt, idx++, r.employee_Contrib);
    sqlite3_bind_double(stmt, idx++, r.employer_Contrib);
    sqlite3_bind_double(stmt, idx++, r.total_Contrib);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.contrib_Type));
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.submissionStatus));
    sqlite3_bind_double(stmt, idx++, r.withHoldingTax);
    sqlite3_bind_int(stmt, idx++, static_cast<int>(r.withHoldingTaxSubmissionStatus));
    sqlite3_bind_text(stmt, idx++, r.lastSubmittedDate.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, idx++, r.submittedByUserId);
    sqlite3_bind_text(stmt, idx++, r.dateCreated.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, idx++, r.dateModified.to_string().c_str(), -1, SQLITE_TRANSIENT);
    return idx;
}