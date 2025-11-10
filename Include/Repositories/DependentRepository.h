#ifndef DEPENDENTREPO_H
#define DEPENDENTREPO_H
#include "BaseRepository.h"
#include "../Models/DataObjects.h"
#include <sqlite3.h>

class DependentRepository: public BaseRepository {
    static Dependent mapDependent(sqlite3_stmt* stmt);
    
    public:
        
        explicit DependentRepository(sqlite3* db);

        std::string getCreateTableSQL() const override;

        //CREATE
        int insertDependent(const Dependent& Dependent);

        // READ
        std::optional<Dependent> getById(int id);             

        // UPDATE
        bool updateDependent(const Dependent& emp); 

        // DELETE
        bool deleteDependent(int id);        
        std::string getLastDependentId();

        bool exists(const std::string& name, const Date& birthday);

};

#endif