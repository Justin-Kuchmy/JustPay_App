#ifndef EMPLOYEESERVICE_H
#define EMPLOYEESERVICE_H
#include "Repositories/EmployeeRepository.h"

class EmployeeService
{
    protected:
        EmployeeRepository& repo;
    public:
        explicit EmployeeService(EmployeeRepository& r);

        //CREATE
        std::string addEmployee(const Employee& employee);

        //READ
        std::optional<Employee> getEmployeeByID(std::string id);

        std::vector<Employee> getAllEmployees();

        //UPDATE
        bool updateEmployee(const Employee& e);
        
        //DELETE
        bool fireEmployee();
};

#endif