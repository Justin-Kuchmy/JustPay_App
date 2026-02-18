#ifndef EMPLOYEEREPO_H
#define EMPLOYEEREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class EmployeeRepository : public BaseRepository
{
    static Employee mapEmployee(sqlite3_stmt *stmt);

public:
    explicit EmployeeRepository(sqlite3 *db);

    bool createTable() const override;

    // CREATE
    std::string insertEmployee(const Employee &Employee);

    // READ
    std::optional<Employee> getById(std::string id);
    std::vector<Employee> getAll();
    std::vector<Employee> findByName(const std::string &name);

    // UPDATE
    bool updateEmployee(const Employee &emp);

    // DELETE
    bool deleteEmployee(std::string id);
    bool deleteAll();
    std::string getLastEmployeeId();
};

#endif