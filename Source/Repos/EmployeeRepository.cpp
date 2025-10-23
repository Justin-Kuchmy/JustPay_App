#include "Repositories/EmployeeRepository.h"
#include "Models/DataObjects.h"
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

                CREATE TRIGGER set_employeeId
				AFTER INSERT ON employees
				FOR EACH ROW
				BEGIN
					UPDATE employees
					SET employeeId = printf('%02d', NEW.department) || '-' || printf('%04d', NEW.tableId)
					WHERE tableId = NEW.tableId;
				END;

                INSERT INTO "employees" (fullName,department,position,jobLevel,status,dateHired,dateSeparation,sssNumber,philHealthNumber,hdmfNumber,monthlyBasicSalary,monthlyAllowances,personalEmail,isActive) VALUES
                ('Alice Santos',0,'HR Manager',2,0,'2022-01-15',NULL,'12-3456789-0','987654321','123456789',45000.0,5000.0,'alice@example.com',1),
                ('Bob Reyes',1,'Finance Associate',0,1,'2023-03-01',NULL,'23-9876543-1','123456789','987654321',28000.0,2000.0,'bob@example.com',1),
                ('Mary Mabulay',1,'Finance',0,0,'2016-01-15',NULL,'11-3159781-0','387254961','148563279',80000.0,5000.0,'mary@example.com',1);

                INSERT INTO "emergency_contacts" (employeeId, name, relation, address, contactNo) VALUES
                ('00-0001','Juan Santos','Brother','Quezon City','09171234567'),
                ('01-0002','Maria Reyes','Mother','Makati','09987654321'),
                ('01-0003','Justin Kuchmy','Husband','CDO','09455554567');

                INSERT INTO "dependents" (employeeId, name, relation, birthday) VALUES
                ('00-0001','Juan Santos','Brother','1991-05-27'),
                ('01-0002','Maria Reyes','Mother','1962-01-12'),
                ('01-0003','Justin Kuchmy','Husband','1991-12-31');

            )";

        };
        bool EmployeeRepository::insertEmployee(const Employee& employee)
        {
            std::string sqlA = std::format(
                "INSERT INTO employees (fullName,department,position,jobLevel,status,dateHired,dateSeparation,sssNumber,philHealthNumber,hdmfNumber,monthlyBasicSalary,monthlyAllowances,personalEmail,isActive) VALUES ('{}', {}, '{}', {}, {}, '{}', '{}', '{}', '{}', '{}', {}, {}, '{}', {})",
                employee.fullName,
                to_int(employee.department),       
                employee.position,
                to_int(employee.jobLevel),         
                to_int(employee.status),           
                to_string(employee.dateHired),     
                to_string(employee.dateSeparation),
                employee.sssNumber,
                employee.philHealthNumber,
                employee.hdmfNumber,
                employee.monthlyBasicSalary,
                employee.monthlyAllowances,
                employee.personalEmail,
                employee.isActive
            );

            if (!execute(sqlA)) return false;
            std::string empId = getLastEmployeeId(); 
            std::string sqlB = std::format("INSERT INTO 'emergency_contacts' (employeeId, name, relation, address, contactNo) VALUES ('{}','{}','{}','{}','{}')", 
                empId,
                employee.emergencyContact.name,
                employee.emergencyContact.relation,
                employee.emergencyContact.address,
                employee.emergencyContact.contactNo

            );
            std::string sqlC = std::format("INSERT INTO dependents (employeeId, name, relation, birthday) VALUES ('{}', '{}','{}','{}')", 
                empId,
                employee.dependent.name,
                employee.dependent.relation,
                to_string(employee.dependent.birthday) // Date -> string (YYYY-MM-DD)
            );

            return execute(sqlB) && execute(sqlC);
        }

        std::string EmployeeRepository::getLastEmployeeId()
        {
            sqlite3_stmt* stmt = nullptr;
            std::string empId;

      
            sqlite3_int64 lastId = sqlite3_last_insert_rowid(db); 

         
            const char* sql = "SELECT employeeId FROM employees WHERE tableId = ?";

            if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK)
            {
                sqlite3_bind_int64(stmt, 1, lastId);

                if (sqlite3_step(stmt) == SQLITE_ROW)
                {
                    const unsigned char* text = sqlite3_column_text(stmt, 0);
                    if (text) empId = reinterpret_cast<const char*>(text);
                }
            }

            sqlite3_finalize(stmt);
            return empId;
        }

        // READ
        std::optional<Employee> EmployeeRepository::getById(std::string id)
        {
            std::string sql = std::format("select * from employees where employeeId = '{}';", id);
            auto results = EmployeeRepository::query<Employee>(sql,mapEmployee);

            if(!results.empty())
            {
                return results.front();
            }
            else
            {
                std::cout << "failed to get anything from the db: ";
                return std::nullopt;
            }
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
                std::cout << "failed to get anything from the db: ";
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
                std::cout << "failed to get anything from the db: ";
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
            return false;
        };// Delete by ID

        bool EmployeeRepository::deleteAll()
        {
            return false;
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
                e.monthlyBasicSalary = sqlite3_column_double(stmt, 12);
                e.monthlyAllowances =  sqlite3_column_double(stmt, 13);
                e.personalEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
                e.isActive =  sqlite3_column_int(stmt, 15);
                return e;
        }