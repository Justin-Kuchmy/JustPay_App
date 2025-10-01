#include "./Headers/EmployeeRepository.hpp"


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

        bool EmployeeRepository::insertEmployee(const Employee& Employee)
        {
            return false;
        };

        // READ
        std::optional<Employee> EmployeeRepository::getById(int id)
        {
            return std::nullopt;
        };// Find one by ID

        std::vector<Employee> EmployeeRepository::getAll()
        {
            return {};
        };// Return all

        std::vector<Employee> EmployeeRepository::findByName(const std::string& name)
        {
            return {};
        }; // Simple search

        // UPDATE
        bool EmployeeRepository::updateEmployee(const Employee& emp)
        {
            return false;
        }; // Update all fields by ID

        // DELETE
        bool EmployeeRepository::deleteEmployee(int id)
        {
            return false;
        };// Delete by ID

        bool EmployeeRepository::deleteAll()
        {
            return false;
        };// Truncate table