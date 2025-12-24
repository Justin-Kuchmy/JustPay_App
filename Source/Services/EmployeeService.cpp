#include "Services/EmployeeService.h"

EmployeeService::EmployeeService(EmployeeRepository& r): repo(r)
{
}

//CREATE
std::string EmployeeService::addEmployee(const Employee& employee)
{
    return this->repo.insertEmployee(employee);
};

//READ
std::optional<Employee> EmployeeService::getEmployeeByID(std::string id)
{
    return this->repo.getById(id);
};

std::vector<Employee> EmployeeService::getAllEmployees()
{
    return this->repo.getAll();
};

//UPDATE
bool EmployeeService::updateEmployee(const Employee& e)
{
    return this->repo.updateEmployee(e);
};

//DELETE
bool EmployeeService::fireEmployee(std::string id)
{
    return this->repo.deleteEmployee(id);
};
