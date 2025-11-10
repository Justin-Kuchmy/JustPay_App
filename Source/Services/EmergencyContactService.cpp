#include "Services/EmergencyContactService.h"
#define DEBUG_LOGS
#include "Utils/Log.h"


EmergencyContactService::EmergencyContactService(EmergencyContactRepository& r): repo(r)
{
};

//CREATE
int EmergencyContactService::addEmergencyContact(const Contact& contact)
{
    int newId = repo.insertContact(contact);
    if (newId <= 0) {
        LOG_DEBUG("[EmergencyContactService] Database insert failed.\n");
        return 0;
    }

    return newId;
};

//READ
std::optional<Contact> EmergencyContactService::getEmergencyContactByID(int id)
{
    return this->repo.getById(id);
};

//UPDATE
bool EmergencyContactService::updateEmergencyContact(const Contact& e)
{
    return this->repo.updateContact(e);

};

//DELETE
bool EmergencyContactService::deleteEmergencyContact(int id)
{
    return this->repo.deleteContact(id);
};
