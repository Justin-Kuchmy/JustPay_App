#include "./Headers/EmployeeRepository.hpp"


        EmployeeRepository::EmployeeRepository() : BaseRepository("payroll.db")
        {
            EmployeeRepository::execute(this->getCreateTableSQL());
        }

        //CREATE
        std::string EmployeeRepository::getCreateTableSQL()const
        {

        };

        bool EmployeeRepository::insertEmployee(const Employee& Employee)
        {

        };

        // READ
        std::optional<Employee> EmployeeRepository::getById(int id)
        {

        };// Find one by ID

        std::vector<Employee> EmployeeRepository::getAll()
        {

        };// Return all

        std::vector<Employee> EmployeeRepository::findByName(const std::string& name)
        {

        }; // Simple search

        // UPDATE
        bool EmployeeRepository::updateEmployee(const Employee& emp)
        {

        }; // Update all fields by ID

        // DELETE
        bool EmployeeRepository::deleteEmployee(int id)
        {

        };// Delete by ID

        bool EmployeeRepository::deleteAll()
        {

        };// Truncate table