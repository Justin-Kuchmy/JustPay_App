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
        bool addDependent(const Dependent& Dependent);

        //READ
        std::optional<Dependent> getDependentByID(std::string id);

        std::vector<Dependent> getAllDependents();

        //UPDATE
        bool updateDependent(const Dependent& e);
        
        //DELETE
        bool deleteDependent(std::string id);
};

#endif