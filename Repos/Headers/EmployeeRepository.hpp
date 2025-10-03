#ifndef EMPLOYEEREPO_HPP
#define EMPLOYEEREPO_HPP
#include "BaseRepository.hpp"
#include "../../Entities/DataObjects.hpp"
#include <sqlite3.h>

class EmployeeRepository: public BaseRepository {
    public: 
        explicit EmployeeRepository();
        
        //CREATE
        std::string getCreateTableSQL() const override;
        bool insertEmployee(const Employee& Employee);

        // READ
        std::optional<Employee> getById(std::string id);             // Find one by ID
        std::vector<Employee> getAll();                      // Return all
        std::vector<Employee> findByName(const std::string& name); // Simple search

        // UPDATE
        bool updateEmployee(const Employee& emp); // Update all fields by ID

        // DELETE
        bool deleteEmployee(std::string id);          // Delete by ID
        bool deleteAll();
        std::string getLastEmployeeId();
        // Truncate table
};

#endif