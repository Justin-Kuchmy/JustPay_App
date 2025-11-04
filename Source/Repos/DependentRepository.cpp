#include "Repositories/DependentRepository.h"


DependentRepository::DependentRepository(sqlite3* db): BaseRepository(db)
{
    std::cout << "\n DependentRepository created" << std::endl;
};

std::string DependentRepository::getCreateTableSQL()const
{
    LOG_DEBUG("DependentRepository::getCreateTableSQL()");
    return R"(
        PRAGMA foreign_keys = OFF;
        DROP TABLE IF EXISTS dependents;

         CREATE TABLE IF NOT EXISTS "dependents" (
            dependentId INTEGER PRIMARY KEY AUTOINCREMENT,
            employeeId TEXT NOT NULL,
            name TEXT NOT NULL,
            relation TEXT NOT NULL,
            birthday TEXT NOT NULL,
            FOREIGN KEY (employeeId) REFERENCES employees(employeeId)
        );


        -- DEPENDENTS (subset for realism)
        INSERT INTO "dependents" (employeeId, name, relation, birthday) VALUES
        ('00-0001','Juan Santos','Brother','1991-05-27'),
        ('00-0016','Andrea Santos','Daughter','2010-09-10'),
        ('01-0002','Maria Reyes','Mother','1962-01-12'),
        ('01-0018','Jose Bautista','Father','1955-11-22'),
        ('02-0004','Andrea Dela Cruz','Wife','1993-02-14'),
        ('02-0004','Mika Dela Cruz','Daughter','2018-06-20'),
        ('02-0020','Nicole Ramos','Sister','1998-03-18'),
        ('03-0006','Camille Gomez','Sister','1995-09-13'),
        ('04-0008','Lorenzo Co','Father','1961-12-05'),
        ('05-0011','Liza Lim','Wife','1992-07-30'),
        ('06-0013','Sofia Javier','Daughter','2017-10-01'),
        ('07-0014','Allan Cruz','Brother','1995-01-15'),
        ('08-0015','Cheryl Chua','Wife','1988-11-11'),
        ('05-0039','Bryan Villena','Brother','1993-05-08'),
        ('02-0034','Ralph Go','Son','2015-10-12');
    )";
};

Dependent DependentRepository::mapDependent(sqlite3_stmt* stmt)
{
    Dependent d;
    d.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    d.relation = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    const unsigned char* text = sqlite3_column_text(stmt, 4);
    if (text) {
        d.birthday = from_string(reinterpret_cast<const char*>(text));
    }
    else {
        d.birthday = Date{1900, 1, 1};
    }
    return d;
};


//CREATE
bool DependentRepository::insertDependent(const Dependent& Dependent)
{
    LOG_DEBUG("DependentRepository::insertDependent not implemented");
    return false;
};

// READ
std::optional<Dependent> DependentRepository::getById(std::string id)
{
    LOG_DEBUG("DependentRepository::getById not implemented");
    return std::nullopt;
};             

// UPDATE
bool DependentRepository::updateDependent(const Dependent& emp)
{
    LOG_DEBUG("DependentRepository::updateDependent not implemented");
    return false;
}; 

// DELETE
bool DependentRepository::deleteDependent(std::string id)
{
    LOG_DEBUG("DependentRepository::deleteDependent not implemented");
    return false;
};        
std::string DependentRepository::getLastDependentId()
{    LOG_DEBUG("DependentRepository::getLastDependentId not implemented");
    return "";

};
