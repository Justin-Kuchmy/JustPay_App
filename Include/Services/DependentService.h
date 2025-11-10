#ifndef DEPENDENTSERVICE_H
#define DEPENDENTSERVICE_H
#include "Repositories/DependentRepository.h"

class DependentService
{
    protected:
        DependentRepository& repo;
    public:
        explicit DependentService(DependentRepository& r);

        //CREATE
        int addDependent(const Dependent& Dependent);

        //READ
        std::optional<Dependent> getDependentByID(int id);

        std::vector<Dependent> getAllDependents();

        //UPDATE
        bool updateDependent(const Dependent& e);
        
        //DELETE
        bool deleteDependent(int id);
 
};

#endif