#include "Services/EmployeeService.h"

EmployeeService::EmployeeService(EmployeeRepository& r): repo(r)
{
    // if(this->repo.createTable())
    // {
    //     qDebug() << "Table Created";
    // }
}

//CREATE
bool EmployeeService::addEmployee(const Employee& employee)
{
    //qDebug() << "Some Functionality to addEmployee";
    qDebug() << employee;
    return this->repo.insertEmployee(employee);
};

//READ
std::optional<Employee> EmployeeService::getEmployeeByID(std::string id)
{
    qDebug() << "Some Functionality to getEmployeeByID";
    return {};
};

std::vector<Employee> EmployeeService::getAllEmployees()
{
    qDebug() << "Some Functionality to getAllEmployees";
    return {};
};

//UPDATE
bool EmployeeService::updateEmployee(const Employee& e)
{
    qDebug() << "Some Functionality to updateEmployee";
    return false;
};

//DELETE
bool EmployeeService::fireEmployee()
{
    qDebug() << "Some Functionality to fireEmployee";
    return false;
};
