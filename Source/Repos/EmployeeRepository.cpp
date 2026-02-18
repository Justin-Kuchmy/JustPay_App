#include "Repositories/EmployeeRepository.h"
#include "Models/DataObjects.h"
#include <format>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Services/AppContext.h"

EmployeeRepository::EmployeeRepository(sqlite3 *db) : BaseRepository(db)
{
    std::cout << "\nEmployeeRepository created" << std::endl;
}

// CREATE
std::string EmployeeRepository::getCreateTableSQL() const
{
    QFile file(":/resources/sql/employee.sql");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        LOG_DEBUG("Failed to open file");
        return "";
    }
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return content.toStdString();
};
std::string EmployeeRepository::insertEmployee(const Employee &employee)
{
    std::string sqlA = std::format("INSERT INTO employees (fullName,department,position,jobLevel,status,dateHired,dateSeparation,sssNumber,philHealthNumber,hdmfNumber,tin,bankAccountNumber,clockInTimeStr, clockOutTimeStr,monthlyBasicSalary,monthlyAllowances,personalEmail,personalMobileNumber,isActive, contactId, dependentId) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    sqlite3_stmt *stmt = nullptr;
    std::string result = "";
    int column{1};
    // database, sql_statement, max_length, out_statement, ptr to unused part of sql string
    if (sqlite3_prepare_v2(db, sqlA.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "SQLite insert failed: " << sqlite3_errmsg(db) << "\n";
        return ""; // Failed to prepare
    }
    sqlite3_bind_text(stmt, column++, employee.fullName.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, column++, static_cast<int>(employee.department));
    sqlite3_bind_text(stmt, column++, employee.position.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, column++, static_cast<int>(employee.jobLevel));
    sqlite3_bind_int(stmt, column++, static_cast<int>(employee.status));
    sqlite3_bind_text(stmt, column++, employee.dateHired.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.dateSeparation.to_string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.sssNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.philHealthNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.hdmfNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.tin.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.bankAccountNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.clockInTimeStr.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.clockOutTimeStr.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, column++, employee.monthlyBasicSalary);
    sqlite3_bind_double(stmt, column++, employee.monthlyAllowances);
    sqlite3_bind_text(stmt, column++, employee.personalEmail.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, column++, employee.personalMobileNumber.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, column++, employee.isActive);
    sqlite3_bind_int(stmt, column++, employee.contactId);
    sqlite3_bind_int(stmt, column++, employee.dependentId);
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "SQLite insert failed: " << sqlite3_errmsg(db) << "\n";
        return 0;
    } // Insert failed
    result = getLastEmployeeId();
    sqlite3_finalize(stmt);
    return result;
}

std::string EmployeeRepository::getLastEmployeeId()
{
    sqlite3_stmt *stmt = nullptr;
    std::string empId;

    // sqlite3_int64 lastId = sqlite3_last_insert_rowid(db);

    const char *sql = R"(
            SELECT employeeId 
            FROM employees 
            WHERE employeeId IS NOT NULL 
            ORDER BY tableId DESC 
            LIMIT 1;
        )";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            empId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return empId;
}

// READ
std::optional<Employee> EmployeeRepository::getById(std::string id)
{
    std::string sql = std::format("select * from employees where employeeId = '{}';", id);
    auto results = EmployeeRepository::query<Employee>(sql, mapEmployee);

    if (results.empty())
    {
        LOG_DEBUG("No employee found for ID: " << id);
        return std::nullopt;
    }

    Employee e = results.front();

    return e;
};

std::vector<Employee> EmployeeRepository::getAll()
{
    std::string sql = std::format("select * from employees;");
    auto results = EmployeeRepository::query<Employee>(sql, mapEmployee);

    if (results.size() > 0)
    {
        return results;
    }
    else
    {
        LOG_DEBUG("failed to get anything from the db");
        return {};
    }
};

std::vector<Employee> EmployeeRepository::findByName(const std::string &name)
{
    std::string sql = std::format("select * from employees where fullName = '{}';", name);
    auto results = EmployeeRepository::query<Employee>(sql, mapEmployee);

    if (results.size() > 0)
    {
        return results;
    }
    else
    {
        LOG_DEBUG("failed to get anything from the db");
        return {};
    }
};

// UPDATE
bool EmployeeRepository::updateEmployee(const Employee &e)
{
    std::string sql = std::format("update employees set fullName='{}', department={}, position='{}', jobLevel={}, status={}, dateHired='{}', dateSeparation='{}', sssNumber='{}', philHealthNumber='{}', hdmfNumber='{}', tin='{}', bankAccountNumber='{}', clockInTimeStr='{}',clockOutTimeStr='{}',monthlyBasicSalary={}, monthlyAllowances={}, personalEmail='{}', isActive=1 where employeeId = '{}'",
                                  e.fullName,
                                  static_cast<int>(e.department),
                                  e.position,
                                  static_cast<int>(e.jobLevel),
                                  static_cast<int>(e.status),
                                  e.dateHired.to_string(),
                                  e.dateSeparation.to_string(),
                                  e.sssNumber,
                                  e.philHealthNumber,
                                  e.hdmfNumber,
                                  e.tin,
                                  e.bankAccountNumber,
                                  e.clockInTimeStr,
                                  e.clockOutTimeStr,
                                  e.monthlyBasicSalary,
                                  e.monthlyAllowances,
                                  e.personalEmail,
                                  e.employeeId);
    return EmployeeRepository::execute(sql);
};

// DELETE
bool EmployeeRepository::deleteEmployee(std::string id)
{
    std::string sql = std::format("update employees set isActive = false where employeeId = '{}'", id);
    return execute(sql);
}; // Delete by ID

bool EmployeeRepository::deleteAll()
{
    std::string sql = "DELETE FROM employees;";
    return execute(sql);
};

Employee EmployeeRepository::mapEmployee(sqlite3_stmt *stmt)
{
    Employee e;
    e.employeeId = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
    e.fullName = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
    e.department = static_cast<Department>(sqlite3_column_int(stmt, 3));
    e.position = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
    e.jobLevel = static_cast<JobLevel>(sqlite3_column_int(stmt, 5));
    e.status = static_cast<EmploymentStatus>(sqlite3_column_int(stmt, 6));
    e.dateHired = Date::fromString(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 7)));

    const unsigned char *text = sqlite3_column_text(stmt, 8);
    if (text)
    {
        e.dateSeparation = Date::fromString(reinterpret_cast<const char *>(text));
    }
    else
    {
        e.dateSeparation = Date{1900, 1, 1};
    }
    e.sssNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 9));
    e.philHealthNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 10));
    e.hdmfNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 11));
    e.tin = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 12));
    e.bankAccountNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 13));
    e.clockInTimeStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 14));
    e.clockOutTimeStr = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 15));
    e.monthlyBasicSalary = sqlite3_column_double(stmt, 16);
    e.monthlyAllowances = sqlite3_column_double(stmt, 17);
    e.personalEmail = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 18));
    e.personalMobileNumber = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 19));
    e.isActive = sqlite3_column_int(stmt, 20);
    e.contactId = sqlite3_column_int(stmt, 21);
    e.dependentId = sqlite3_column_int(stmt, 22);

    return e;
}