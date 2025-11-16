#include "Repositories/EmployeeRepository.h"
#include "Models/DataObjects.h"
#include <format>
#define DEBUG_LOGS
#include "Utils/Log.h"
#include "Services/AppContext.h"


EmployeeRepository::EmployeeRepository(sqlite3* db) : BaseRepository(db)
{
    std::cout << "\nEmployeeRepository created" << std::endl;
}

//CREATE
std::string EmployeeRepository::getCreateTableSQL()const
{
    return R"(

        PRAGMA foreign_keys = OFF;


        DROP TABLE IF EXISTS employees;


        CREATE TABLE IF NOT EXISTS employees (
            tableId INTEGER PRIMARY KEY AUTOINCREMENT,
            employeeId TEXT UNIQUE,
            fullName TEXT NOT NULL,
            department INTEGER NOT NULL,
            position TEXT,
            jobLevel INTEGER NOT NULL,
            status INTEGER NOT NULL,
            dateHired TEXT NOT NULL,
            dateSeparation TEXT,
            sssNumber TEXT,
            philHealthNumber TEXT,
            hdmfNumber TEXT,
            tin TEXT,
            bankAccountNumber TEXT,
            monthlyBasicSalary REAL,
            monthlyAllowances REAL,
            personalEmail TEXT,
            personalMobileNumber TEXT,
            isActive INTEGER,
            contactId INTEGER,
            dependentId INTEGER,
            FOREIGN KEY (contactId) REFERENCES emergency_contacts(contactId) ON DELETE SET NULL ON UPDATE CASCADE,
            FOREIGN KEY (dependentId) REFERENCES dependents(dependentId) ON DELETE SET NULL ON UPDATE CASCADE

        );


        CREATE TRIGGER set_employeeId
        AFTER INSERT ON employees
        FOR EACH ROW
        BEGIN
            UPDATE employees
            SET employeeId = printf('%02d', NEW.department) || '-' || printf('%04d', NEW.tableId)
            WHERE tableId = NEW.tableId;
        END;

       -- EMPLOYEES (50 entries)
        INSERT INTO "employees" (fullName, department, position, jobLevel, status, dateHired, dateSeparation,sssNumber, philHealthNumber, hdmfNumber,tin, bankAccountNumber,monthlyBasicSalary, monthlyAllowances, personalEmail, personalMobileNumber, isActive, contactId, dependentId)
        VALUES
        ('Alice Santos',0,'HR Manager',2,0,'2022-01-15',NULL,'12-3456789-0','987654321','123456789','123-456-789','001234567890',45000,5000,'alice@example.com','09170010001',1,1,1),
        ('Bob Reyes',1,'Finance Associate',0,1,'2023-03-01',NULL,'23-9876543-1','123456789','987654321','234-567-890','001234567891',28000,2000,'bob@example.com','09170010002',1,2,2),
        ('Mary Mabulay',1,'Senior Accountant',1,0,'2016-01-15',NULL,'11-3159781-0','387254961','148563279','345-678-901','001234567892',80000,5000,'mary@example.com','09170010003',1,3,3),
        ('Carlos Dela Cruz',2,'Software Engineer',1,0,'2021-05-03',NULL,'14-1234567-9','111222333','555666777','456-789-012','001234567893',55000,3000,'carlos@company.com','09170010004',1,4,4),
        ('Janine Uy',2,'UI/UX Designer',0,1,'2022-11-10',NULL,'17-7654321-0','444555666','333222111','567-890-123','001234567894',42000,2500,'janine@company.com','09170010005',1,5,5),
        ('Patrick Gomez',3,'Marketing Specialist',0,0,'2020-02-14',NULL,'22-1112223-4','777888999','555444333','678-901-234','001234567895',38000,1500,'patrick@company.com','09170010006',1,6,6),
        ('Lea Navarro',3,'Marketing Manager',2,0,'2018-08-01',NULL,'25-9998887-6','222111000','999888777','789-012-345','001234567896',65000,5000,'lea@company.com','09170010007',1,7,7),
        ('Jasmine Co',4,'Admin Assistant',0,1,'2021-12-20',NULL,'30-6543210-9','555444333','222333444','890-123-456','001234567897',25000,1200,'jasmine@company.com','09170010008',1,8,8),
        ('Miguel Tan',4,'Operations Supervisor',1,0,'2019-03-25',NULL,'31-1230987-4','666777888','111222333','901-234-567','001234567898',47000,3000,'miguel@company.com','09170010009',1,9,9),
        ('Katrina Ramos',5,'IT Support',0,1,'2023-06-05',NULL,'32-3216549-8','555666777','888999000','012-345-678','001234567899',30000,1500,'katrina@company.com','09170010010',1,10,10);
    )";

};
std::string EmployeeRepository::insertEmployee(const Employee& employee)
{
    std::string sqlA = std::format("INSERT INTO employees (fullName,department,position,jobLevel,status,dateHired,dateSeparation,sssNumber,philHealthNumber,hdmfNumber,tin,bankAccountNumber,monthlyBasicSalary,monthlyAllowances,personalEmail,personalMobileNumber,isActive, contactId, dependentId) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    sqlite3_stmt* stmt = nullptr;
    std::string result = "";
    //database, sql_statement, max_length, out_statement, ptr to unused part of sql string
    if (sqlite3_prepare_v2(db, sqlA.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return "";  // Failed to prepare
    sqlite3_bind_text(stmt, 1, employee.fullName.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_int(stmt, 2, static_cast<int>(employee.department));    
    sqlite3_bind_text(stmt, 3, employee.position.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_int(stmt, 4, static_cast<int>(employee.jobLevel));    
    sqlite3_bind_int(stmt, 5, static_cast<int>(employee.status));    
    sqlite3_bind_text(stmt, 6, employee.dateHired.to_string().c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 7, employee.dateSeparation.to_string().c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 8, employee.sssNumber.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 9, employee.philHealthNumber.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 10, employee.hdmfNumber.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 11, employee.tin.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 12, employee.bankAccountNumber.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_double(stmt, 13, employee.monthlyBasicSalary);    
    sqlite3_bind_double(stmt, 14, employee.monthlyAllowances);    
    sqlite3_bind_text(stmt, 15, employee.personalEmail.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 16, employee.personalMobileNumber.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_int(stmt, 17, employee.isActive);  
    sqlite3_bind_int(stmt, 18, employee.contactId);
    sqlite3_bind_int(stmt, 19, employee.dependentId);
    int rc = sqlite3_step(stmt);
    result = getLastEmployeeId();
    sqlite3_finalize(stmt);
    return result;
}

std::string EmployeeRepository::getLastEmployeeId()
{
    sqlite3_stmt* stmt = nullptr;
    std::string empId;


    //sqlite3_int64 lastId = sqlite3_last_insert_rowid(db); 

    
    const char* sql = R"(
            SELECT employeeId 
            FROM employees 
            WHERE employeeId IS NOT NULL 
            ORDER BY tableId DESC 
            LIMIT 1;
        )";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
            empId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    }

    sqlite3_finalize(stmt);
    return empId;
}

// READ
std::optional<Employee> EmployeeRepository::getById(std::string id)
{
    std::string sql = std::format("select * from employees where employeeId = '{}';", id);
    auto results = EmployeeRepository::query<Employee>(sql,mapEmployee);

    if (results.empty())
    {
        LOG_DEBUG("No employee found for ID: "<< id);
        return std::nullopt;
    }

    Employee e = results.front();

    return e;

};

std::vector<Employee> EmployeeRepository::getAll()
{
    std::string sql = std::format("select * from employees;");
    auto results = EmployeeRepository::query<Employee>(sql,mapEmployee);

    if(results.size() > 0)
    {
        return results;
    }
    else
    {
        LOG_DEBUG("failed to get anything from the db");
        return {};
    }
};

std::vector<Employee> EmployeeRepository::findByName(const std::string& name)
{
    std::string sql = std::format("select * from employees where fullName = '{}';", name);
    auto results = EmployeeRepository::query<Employee>(sql,mapEmployee);

    if(results.size() > 0)
    {
        return results;
    }
    else
    {
        LOG_DEBUG( "failed to get anything from the db");
        return {};
    }
}; 

// UPDATE
bool EmployeeRepository::updateEmployee(const Employee& e)
{            
    std::string sql = std::format("update employees set fullName='{}', department={}, position='{}', jobLevel={}, status={}, dateHired='{}', dateSeparation='{}', sssNumber='{}', philHealthNumber='{}', hdmfNumber='{}', monthlyBasicSalary={}, monthlyAllowances={}, personalEmail='{}', isActive=1 where employeeId = '{}'",
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
        e.monthlyBasicSalary,
        e.monthlyAllowances,
        e.personalEmail,
        e.employeeId
        );
    return EmployeeRepository::execute(sql);
}; 

// DELETE
bool EmployeeRepository::deleteEmployee(std::string id)
{
    std::string sql = std::format("update employees set isActive = false where employeeId = '{}'", id);
    return execute(sql);
};// Delete by ID

bool EmployeeRepository::deleteAll()
{
    std::string sql = "DELETE FROM employees;";
    return execute(sql);
};

Employee EmployeeRepository::mapEmployee(sqlite3_stmt* stmt)
{
        Employee e;
        e.employeeId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        e.fullName =   reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        e.department = static_cast<Department>(sqlite3_column_int(stmt, 3));  
        e.position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        e.jobLevel =  static_cast<JobLevel>(sqlite3_column_int(stmt, 5));  
        e.status =     static_cast<EmploymentStatus>(sqlite3_column_int(stmt, 6));     
        e.dateHired =    Date::fromString(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));   

        const unsigned char* text = sqlite3_column_text(stmt, 8);
        if (text) {
            e.dateSeparation = Date::fromString(reinterpret_cast<const char*>(text));
        } else {
            e.dateSeparation = Date{1900,1,1};
        }
        e.sssNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
        e.philHealthNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
        e.hdmfNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
        e.tin = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        e.bankAccountNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
        e.monthlyBasicSalary = sqlite3_column_double(stmt, 14);
        e.monthlyAllowances =  sqlite3_column_double(stmt, 15);
        e.personalEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 16));
        e.personalMobileNumber  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 17));
        e.isActive =  sqlite3_column_int(stmt, 18);
        e.contactId = sqlite3_column_int(stmt, 19);
        e.dependentId = sqlite3_column_int(stmt, 20);

        return e;
}