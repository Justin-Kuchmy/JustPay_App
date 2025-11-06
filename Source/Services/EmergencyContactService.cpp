#include "Services/EmergencyContactService.h"


EmergencyContactService::EmergencyContactService(EmergencyContactRepository& r): repo(r)
{
};

//CREATE
bool EmergencyContactService::addEmergencyContact(const Contact& contact)
{
    return this->repo.insertContact(contact);
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
