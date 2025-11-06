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
    LOG_DEBUG("EmployeeRepository::getCreateTableSQL()");
    return R"(

        PRAGMA foreign_keys = OFF;


        DROP TABLE IF EXISTS employees;


        CREATE TABLE IF NOT EXISTS "employees" (
            tableId INTEGER PRIMARY KEY AUTOINCREMENT,
            employeeId TEXT UNIQUE,
            fullName TEXT NOT NULL,
            department INTEGER NOT NULL,
            position TEXT,
            jobLevel INTEGER NOT NULL,
            status INTEGER NOT NULL,
            dateHired TEXT,
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
            isActive INTEGER
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
INSERT INTO "employees" (fullName, department, position, jobLevel, status, dateHired, dateSeparation,sssNumber, philHealthNumber, hdmfNumber,tin, bankAccountNumber,monthlyBasicSalary, monthlyAllowances, personalEmail, personalMobileNumber, isActive)
VALUES
('Alice Santos',0,'HR Manager',2,0,'2022-01-15',NULL,'12-3456789-0','987654321','123456789','123-456-789','001234567890',45000,5000,'alice@example.com','09170010001',1),
('Bob Reyes',1,'Finance Associate',0,1,'2023-03-01',NULL,'23-9876543-1','123456789','987654321','234-567-890','001234567891',28000,2000,'bob@example.com','09170010002',1),
('Mary Mabulay',1,'Senior Accountant',1,0,'2016-01-15',NULL,'11-3159781-0','387254961','148563279','345-678-901','001234567892',80000,5000,'mary@example.com','09170010003',1),
('Carlos Dela Cruz',2,'Software Engineer',1,0,'2021-05-03',NULL,'14-1234567-9','111222333','555666777','456-789-012','001234567893',55000,3000,'carlos@company.com','09170010004',1),
('Janine Uy',2,'UI/UX Designer',0,1,'2022-11-10',NULL,'17-7654321-0','444555666','333222111','567-890-123','001234567894',42000,2500,'janine@company.com','09170010005',1),
('Patrick Gomez',3,'Marketing Specialist',0,0,'2020-02-14',NULL,'22-1112223-4','777888999','555444333','678-901-234','001234567895',38000,1500,'patrick@company.com','09170010006',1),
('Lea Navarro',3,'Marketing Manager',2,0,'2018-08-01',NULL,'25-9998887-6','222111000','999888777','789-012-345','001234567896',65000,5000,'lea@company.com','09170010007',1),
('Jasmine Co',4,'Admin Assistant',0,1,'2021-12-20',NULL,'30-6543210-9','555444333','222333444','890-123-456','001234567897',25000,1200,'jasmine@company.com','09170010008',1),
('Miguel Tan',4,'Operations Supervisor',1,0,'2019-03-25',NULL,'31-1230987-4','666777888','111222333','901-234-567','001234567898',47000,3000,'miguel@company.com','09170010009',1),
('Katrina Ramos',5,'IT Support',0,1,'2023-06-05',NULL,'32-3216549-8','555666777','888999000','012-345-678','001234567899',30000,1500,'katrina@company.com','09170010010',1),
('Diego Lim',5,'Network Engineer',1,0,'2020-10-18',NULL,'35-2221110-5','777666555','333444222','123-456-780','001234567900',52000,2000,'diego@company.com','09170010011',1),
('Elaine Robles',6,'Sales Associate',0,1,'2022-09-30',NULL,'37-1113332-4','999000111','555777888','234-567-891','001234567901',28000,1000,'elaine@company.com','09170010012',1),
('Ronald Javier',6,'Sales Manager',2,0,'2017-07-17',NULL,'38-4445556-7','888777666','222111333','345-678-902','001234567902',72000,4000,'ronald@company.com','09170010013',1),
('Samantha Cruz',7,'Legal Officer',1,0,'2021-04-12',NULL,'40-3214567-2','333222111','111222333','456-789-013','001234567903',60000,3500,'samantha@company.com','09170010014',1),
('Victor Chua',8,'Procurement Officer',1,0,'2019-09-05',NULL,'41-7896541-3','555444333','999888777','567-890-124','001234567904',48000,2200,'victor@company.com','09170010015',1),
('Andrea Reyes',0,'HR Associate',0,1,'2023-02-01',NULL,'42-1112223-1','888111222','777666555','678-901-235','001234567905',31000,1500,'andrea@company.com','09170010016',1),
('Rico Fernandez',0,'Recruitment Specialist',1,0,'2021-06-20',NULL,'43-4443332-9','555222111','999888000','789-012-346','001234567906',38000,1800,'rico@company.com','09170010017',1),
('Jocelyn Bautista',1,'Payroll Officer',1,0,'2020-09-15',NULL,'44-9876541-0','333444555','222111000','890-123-457','001234567907',50000,2500,'jocelyn@company.com','09170010018',1),
('Martin Cruz',1,'Audit Analyst',0,1,'2023-04-10',NULL,'45-1118887-2','111222333','555444666','901-234-568','001234567908',34000,1500,'martin@company.com','09170010019',1),
('Nicole David',2,'Frontend Developer',1,0,'2020-07-18',NULL,'46-9995553-7','222111333','333222111','012-345-679','001234567909',54000,2000,'nicole@company.com','09170010020',1),
('Albert Chan',2,'Backend Developer',1,0,'2021-01-05',NULL,'47-5556667-8','444555666','111222333','123-456-790','001234567910',58000,2500,'albert@company.com','09170010021',1),
('Gloria Ramos',3,'Content Writer',0,1,'2022-05-23',NULL,'48-1114442-1','666777888','999000111','234-567-801','001234567911',30000,1200,'gloria@company.com','09170010022',1),
('Harold Diaz',3,'Digital Strategist',1,0,'2020-02-14',NULL,'49-3335554-9','888777666','555444333','345-678-912','001234567912',47000,2000,'harold@company.com','09170010023',1),
('Isabel Lim',4,'Administrative Officer',1,0,'2019-10-08',NULL,'50-6668889-2','999888777','444333222','456-789-023','001234567913',38000,1800,'isabel@company.com','09170010024',1),
('Jerome Villanueva',4,'Logistics Assistant',0,1,'2023-01-17',NULL,'51-3331114-3','777666555','222333444','567-890-134','001234567914',29000,1200,'jerome@company.com','09170010025',1),
('Kelly Santos',5,'Systems Analyst',1,0,'2018-12-05',NULL,'52-1113335-5','444333222','555666777','678-901-245','001234567915',62000,3000,'kelly@company.com','09170010026',1),
('Louis Tan',5,'Database Admin',1,0,'2021-11-22',NULL,'53-9991116-6','666777888','111000999','789-012-356','001234567916',59000,2800,'louis@company.com','09170010027',1),
('Mia Hernandez',6,'Sales Executive',0,1,'2023-03-02',NULL,'54-5553337-7','777666555','222111333','890-123-467','001234567917',32000,1500,'mia@company.com','09170010028',1),
('Noel Perez',6,'Account Manager',1,0,'2020-07-11',NULL,'55-1114448-8','888777666','999888777','901-234-578','001234567918',56000,3000,'noel@company.com','09170010029',1),
('Olivia Garcia',7,'Paralegal',0,1,'2022-09-30',NULL,'56-2225559-9','333222111','444333222','012-345-689','001234567919',35000,1500,'olivia@company.com','09170010030',1),
('Paulo Santos',7,'Legal Researcher',0,1,'2021-08-19',NULL,'57-77788>80-0','555444333','666555444','123-456-790','001234567920',33000,1200,'paulo@company.com','09170010031',1),
('Queenie Dizon',8,'Procurement Assistant',0,1,'2022-10-05',NULL,'58-3339991-1','999888777','222111000','234-567-801','001234567921',34000,1000,'queenie@company.com','09170010032',1),
('Ralph Go',8,'Purchasing Officer',1,0,'2020-03-16',NULL,'59-4441112-2','666555444','333222111','345-678-912','001234567922',52000,2500,'ralph@company.com','09170010033',1),
('Sofia Tan',2,'Mobile Developer',1,0,'2021-02-12',NULL,'60-7773333-3','555444333','999888777','456-789-023','001234567923',58000,2500,'sofia@company.com','09170010034',1),
('Timothy Cruz',5,'Security Analyst',1,0,'2019-07-25',NULL,'61-2224444-4','444333222','111222333','567-890-134','001234567924',61000,2800,'timothy@company.com','09170010035',1),
('Ulyssa Bautista',3,'PR Coordinator',0,1,'2022-11-04',NULL,'62-3335555-5','333222111','999000111','678-901-245','001234567925',31000,1200,'ulyssa@company.com','09170010036',1),
('Victorino Yap',8,'Inventory Clerk',0,1,'2023-05-19',NULL,'63-4446666-6','888777666','222111000','789-012-356','001234567926',27000,800,'victorino@company.com','09170010037',1),
('Wendy Chan',0,'HR Assistant',0,1,'2021-03-03',NULL,'64-5557777-7','999888777','555444333','890-123-467','001234567927',31000,1500,'wendy@company.com','09170010038',1),
('Xavier Co',1,'Budget Analyst',1,0,'2018-10-15',NULL,'65-6668888-8','777666555','333222111','901-234-578','001234567928',55000,2500,'xavier@company.com','09170010039',1),
('Yvonne Perez',2,'QA Tester',0,1,'2022-08-01',NULL,'66-7779999-9','555444333','111222333','012-345-689','001234567929',40000,1500,'yvonne@company.com','09170010040',1),
('Zandro Lim',3,'Marketing Coordinator',0,1,'2023-06-09',NULL,'67-8880000-0','444333222','222111333','123-456-790','001234567930',29000,1000,'zandro@company.com','09170010041',1),
('Abigail Dela Torre',4,'Office Clerk',0,1,'2023-01-25',NULL,'68-9991111-1','333222111','555444666','234-567-801','001234567931',26000,900,'abigail@company.com','09170010042',1),
('Bryan Villena',5,'DevOps Engineer',1,0,'2020-06-30',NULL,'69-1112222-2','666777888','111222333','345-678-912','001234567932',63000,3000,'bryan@company.com','09170010043',1),
('Clarisse Lee',6,'Sales Coordinator',0,1,'2022-04-21',NULL,'70-2223333-3','999888777','555444333','456-789-023','001234567933',31000,1200,'clarisse@company.com','09170010044',1),
('Derrick Ramos',7,'Compliance Officer',1,0,'2019-09-02',NULL,'71-33>34444-4','888777666','333222111','567-890-134','001234567934',57000,2800,'derrick@company.com','09170010045',1),
('Evelyn Ong',8,'Supply Chain Analyst',1,0,'2020-08-12',NULL,'72-4445555-5','777666555','111222333','678-901-245','001234567935',53000,2300,'evelyn@company.com','09170010046',1),
('Francis Tiu',0,'Training Officer',1,0,'2021-07-07',NULL,'73-5556666-6','555444333','222111000','789-012-356','001234567936',37000,1500,'francis@company.com','09170010047',1),
('Georgia Uy',1,'Treasury Assistant',0,1,'2023-05-10',NULL,'74-6667777-7','444333222','333222111','890-123-467','001234567937',33000,1200,'georgia@company.com','09170010048',1),
('Henry Bautista',2,'Cloud Engineer',1,0,'2020-02-11',NULL,'75-7778888-8','333222111','111222333','901-234-578','001234567938',65000,3000,'henry@company.com','09170010049',1);





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
    sqlite3_bind_int(stmt, 2, to_int(employee.department));    
    sqlite3_bind_text(stmt, 3, employee.position.c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_int(stmt, 4, to_int(employee.jobLevel));    
    sqlite3_bind_int(stmt, 5, to_int(employee.status));    
    sqlite3_bind_text(stmt, 6, to_string(employee.dateHired).c_str(), -1, SQLITE_TRANSIENT);    
    sqlite3_bind_text(stmt, 7, to_string(employee.dateSeparation).c_str(), -1, SQLITE_TRANSIENT);    
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
        to_int(e.department),       
        e.position,
        to_int(e.jobLevel),         
        to_int(e.status),           
        to_string(e.dateHired),     
        to_string(e.dateSeparation),
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
        e.department = to_department(sqlite3_column_int(stmt, 3));  
        e.position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        e.jobLevel =  to_jobLevel(sqlite3_column_int(stmt, 5));  
        e.status =     to_status(sqlite3_column_int(stmt, 6));     
        e.dateHired =    from_string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));   

        const unsigned char* text = sqlite3_column_text(stmt, 8);
        if (text) {
            e.dateSeparation = from_string(reinterpret_cast<const char*>(text));
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