#include "Repositories/EmployeeRepository.h"

class EmployeeService
{
    EmployeeRepository& repo;

    public:
        EmployeeService(EmployeeRepository* r): repo(*r){};

        //CREATE
        void addEmployee();

        //READ
        void getEmployeeByID();

        void getAllEmployees();

        //UPDATE
        void updateEmployee();
        
        //DELETE
        void fireEmployee();
};