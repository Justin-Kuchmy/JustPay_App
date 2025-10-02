#include "./Headers/EmployeeRepository.hpp"
#include <format>


        EmployeeRepository::EmployeeRepository() : BaseRepository("payroll.db")
        {
        }

        //CREATE
        std::string EmployeeRepository::getCreateTableSQL()const
        {
            return R"(

                PRAGMA foreign_keys = OFF;

                DROP TABLE IF EXISTS emergency_contacts;
                DROP TABLE IF EXISTS dependents;
                DROP TABLE IF EXISTS employees;


                CREATE TABLE IF NOT EXISTS "employees" (
                    employeeId TEXT PRIMARY KEY,
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
                    monthlyBasicSalary REAL,
                    monthlyAllowances REAL,
                    personalEmail TEXT,
                    isActive INTEGER
                );

                CREATE TABLE IF NOT EXISTS emergency_contacts (
                    contactId INTEGER PRIMARY KEY AUTOINCREMENT,
                    employeeId TEXT NOT NULL,
                    name TEXT,
                    relation TEXT,
                    address TEXT,
                    contactNo TEXT,
                    FOREIGN KEY (employeeId) REFERENCES employees(employeeId)
                );

                CREATE TABLE IF NOT EXISTS "dependents" (
                    dependentId INTEGER PRIMARY KEY AUTOINCREMENT,
                    employeeId TEXT NOT NULL,
                    name TEXT NOT NULL,
                    relation TEXT NOT NULL,
                    birthday TEXT NOT NULL,
                    FOREIGN KEY (employeeId) REFERENCES employees(employeeId)
                );

                INSERT INTO "employees" VALUES
                ('01-0001','Alice Santos',0,'HR Manager',2,0,'2022-01-15',NULL,'12-3456789-0','987654321','123456789',45000.0,5000.0,'alice@example.com',1),
                ('02-0002','Bob Reyes',1,'Finance Associate',0,1,'2023-03-01',NULL,'23-9876543-1','123456789','987654321',28000.0,2000.0,'bob@example.com',1),
                ('02-0003','Mary Mabulay',1,'Finance',0,0,'2016-01-15',NULL,'11-3159781-0','387254961','148563279',80000.0,5000.0,'mary@example.com',1);

                INSERT INTO "emergency_contacts" (employeeId, name, relation, address, contactNo) VALUES
                ('01-0001','Juan Santos','Brother','Quezon City','09171234567'),
                ('02-0002','Maria Reyes','Mother','Makati','09987654321'),
                ('02-0003','Justin Kuchmy','Husband','CDO','09455554567');

                INSERT INTO "dependents" (employeeId, name, relation, birthday) VALUES
                ('01-0001','Juan Santos','Brother','1991-05-27'),
                ('02-0002','Maria Reyes','Mother','1962-01-12'),
                ('02-0003','Justin Kuchmy','Husband','1991-12-31');

            )";

        };
        bool EmployeeRepository::insertEmployee(const Employee& employee)
        {
            std::string sql = std::format(
                "INSERT INTO employees VALUES ('{}', '{}', {}, '{}', {}, {}, '{}', '{}', '{}', '{}', '{}', {}, {}, '{}', {})",
                employee.employeeId,
                employee.fullName,
                to_int(employee.department),       // enum -> int
                employee.position,
                to_int(employee.jobLevel),         // enum -> int
                to_int(employee.status),           // enum -> int
                to_string(employee.dateHired),     // Date -> string (YYYY-MM-DD)
                to_string(employee.dateSeparation),
                employee.sssNumber,
                employee.philHealthNumber,
                employee.hdmfNumber,
                employee.monthlyBasicSalary,
                employee.monthlyAllowances,
                employee.personalEmail,
                employee.isActive
            );

            return execute(sql);
        }

        // READ
        std::optional<Employee> EmployeeRepository::getById(std::string id)
        {
            std::string sql = std::format("select * from employees where employeeId = '{}';", id);
            auto results = EmployeeRepository::query<Employee>(
            sql,
            [](sqlite3_stmt* stmt) -> Employee {
                Employee e;
                e.employeeId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
                e.fullName =   reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
                e.department = to_department(sqlite3_column_int(stmt, 2));  
                e.position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
                e.jobLevel =  to_jobLevel(sqlite3_column_int(stmt, 4));  
                e.status =     to_status(sqlite3_column_int(stmt, 5));     
                e.dateHired =    from_string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6)));   

                const unsigned char* text = sqlite3_column_text(stmt, 7);
                if (text) {
                    e.dateSeparation = from_string(reinterpret_cast<const char*>(text));
                } else {
                    e.dateSeparation = Date{1900,1,1};
                }
                e.sssNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
                e.philHealthNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
                e.hdmfNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
                e.monthlyBasicSalary = sqlite3_column_double(stmt, 11);
                e.monthlyAllowances =  sqlite3_column_double(stmt, 12);
                e.personalEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
                e.isActive =  sqlite3_column_int(stmt, 14);
                return e;
            });

            if(!results.empty())
            {
                return results.front();
            }
            else
            {
                std::cout << "failed to get anything from the db: ";
                return std::nullopt;
            }
        };// Find one by ID

        std::vector<Employee> EmployeeRepository::getAll()
        {
            std::string sql = std::format("select * from employees;");
            return {};
        };// Return all

        std::vector<Employee> EmployeeRepository::findByName(const std::string& name)
        {
            std::string sql = std::format("select * from employeee where fullName = '{}';", name);
            return {};
        }; // Simple search

        // UPDATE
        bool EmployeeRepository::updateEmployee(const Employee& emp)
        {
            std::string sql = std::format("SELECT * from employeee where employeeId = '{}'", emp.employeeId);
            return false;
        }; // Update all fields by ID

        // DELETE
        bool EmployeeRepository::deleteEmployee(std::string id)
        {
            std::string sql = std::format("update employees set isActive = false where employeeId = '{}'", id);
            return false;
        };// Delete by ID

        bool EmployeeRepository::deleteAll()
        {
            return false;
        };// Truncate table