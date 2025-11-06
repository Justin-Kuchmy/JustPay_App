#ifndef EMERGENCYCONTACTSERVICE_H
#define EMERGENCYCONTACTSERVICE_H
#include "Repositories/EmergencyContactRepository.h"

class EmergencyContactService
{
    protected:
        EmergencyContactRepository& repo;
    public:
        explicit EmergencyContactService(EmergencyContactRepository& r);

        //CREATE
        bool addEmergencyContact(const Contact& EmergencyContact);

        //READ
        std::optional<Contact> getEmergencyContactByID(int id);

        std::vector<Contact> getAllEmergencyContacts();

        //UPDATE
        bool updateEmergencyContact(const Contact& e);
        
        //DELETE
        bool deleteEmergencyContact(int id);
};

#endif