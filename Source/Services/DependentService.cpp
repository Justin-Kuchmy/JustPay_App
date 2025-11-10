#include "Services/DependentService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"




DependentService::DependentService(DependentRepository& r): repo(r)
{

};

//CREATE
int DependentService::addDependent(const Dependent& dependent)
{
    if (dependent.name.empty() || dependent.relation.empty()) {
        LOG_DEBUG("[DependentService] Name or relation cannot be empty.\n");
        return 0;
    }

    auto today = Date::getTodayDate();
    if (dependent.birthday > today) {
        LOG_DEBUG("[DependentService] Birthday cannot be in the future.\n");
        return 0;
    }

    if (this->repo.exists(dependent.name, dependent.birthday) ) {
        LOG_DEBUG("[DependentService] Duplicate dependent detected.\n");
        return 0;
    }

    // If all good, insert and return the new ID
    int newId = repo.insertDependent(dependent);
    if (newId <= 0) {
        LOG_DEBUG("[DependentService] Database insert failed.\n");
        return 0;
    }

    return newId;
};

//READ
std::optional<Dependent> DependentService::getDependentByID(int id)
{
return this->repo.getById(id);
};

//UPDATE
bool DependentService::updateDependent(const Dependent& d)
{
return this->repo.updateDependent(d);
};

//DELETE
bool DependentService::deleteDependent(int id)
{
return this->repo.deleteDependent(id);
};
