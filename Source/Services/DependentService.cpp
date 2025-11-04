#include "Services/DependentService.h"


DependentService::DependentService(DependentRepository& r): repo(r)
{

};

//CREATE
bool DependentService::addDependent(const Dependent& dependent)
{
return this->repo.insertDependent(dependent);
};

//READ
std::optional<Dependent> DependentService::getDependentByID(std::string id)
{
return this->repo.getById(id);
};

//UPDATE
bool DependentService::updateDependent(const Dependent& d)
{
return this->repo.updateDependent(d);
};

//DELETE
bool DependentService::deleteDependent(std::string id)
{
return this->repo.deleteDependent(id);
};
