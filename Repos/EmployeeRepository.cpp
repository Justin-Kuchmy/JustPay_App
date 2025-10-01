#include "./Headers/EmployeeRepository.hpp"


        EmployeeRepository::EmployeeRepository() : BaseRepository("payroll.db")
        {
        }

        //CREATE
        std::string EmployeeRepository::getCreateTableSQL()const
        {
            return R"(
                CREATE TABLE IF NOT EXISTS employees (
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

                CREATE TABLE IF NOT EXISTS dependents (
                    dependentId INTEGER PRIMARY KEY AUTOINCREMENT,
                    employeeId TEXT NOT NULL,
                    name TEXT NOT NULL,
                    relation TEXT NOT NULL,
                    birthday TEXT NOT NULL,
                    FOREIGN KEY (employeeId) REFERENCES employees(employeeId)
                );
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