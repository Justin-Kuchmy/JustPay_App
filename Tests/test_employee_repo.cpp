#include <gtest/gtest.h>
#include "Repositories/EmployeeRepository.h"
#include "Repositories/DependentRepository.h"
#include "Repositories/EmergencyContactRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"

using std::cout;
using std::endl;

static void runSqlScript(sqlite3 *db, const std::string &sqlFilePath)
{
    std::ifstream file(sqlFilePath);
    ASSERT_TRUE(file.is_open()) << "Failed to open SQL file: " << sqlFilePath;
    std::string sql((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    ASSERT_EQ(rc, SQLITE_OK) << "SQL error: " << (errMsg ? errMsg : "");
    if (errMsg)
        sqlite3_free(errMsg);
};

class EmployeeRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<EmployeeRepository> empRepo;
    std::unique_ptr<DependentRepository> depRepo;
    std::unique_ptr<EmergencyContactRepository> contactRepo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        empRepo = std::make_unique<EmployeeRepository>(db);
        depRepo = std::make_unique<DependentRepository>(db);
        contactRepo = std::make_unique<EmergencyContactRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(EmployeeRepoTest, InsertEmployee_AddsNewRecord)
{
    Contact newContact;
    newContact.name = "cont_Name";
    newContact.relation = "cont_Relation";
    newContact.contactNo = "cont_Number";
    newContact.address = "cont_Addr";

    // int newContactId = contactRepo->insertContact(newContact);

    Dependent newDependent;
    newDependent.name = "dep_Name";
    newDependent.relation = "dep_Relation";
    newDependent.birthday = Date::fromString("2001-01-01");

    // int newDependentId = depRepo->insertDependent(newDependent);

    Employee newEmp;
    newEmp.fullName = "Bob Reyes";
    newEmp.department = static_cast<Department>(1);
    newEmp.position = "Accountant";
    newEmp.jobLevel = static_cast<JobLevel>(1);
    newEmp.status = static_cast<EmploymentStatus>(0);
    newEmp.dateHired = Date::fromString("2023-03-01");
    newEmp.sssNumber = "34-5678901-2";
    newEmp.philHealthNumber = "12-345678901-2";
    newEmp.hdmfNumber = "1234-5678-9012";
    newEmp.tin = "123-456-789";
    newEmp.bankAccountNumber = "001234567890";
    newEmp.clockInTimeStr = "7:30";
    newEmp.clockOutTimeStr = "18:30";
    newEmp.monthlyBasicSalary = 30000.0;
    newEmp.monthlyAllowances = 2000.0;
    newEmp.personalEmail = "bob.reyes@example.com";
    newEmp.personalMobileNumber = "09171234567";
    newEmp.isActive = true;
    newEmp.contactId = newContact.contactId;
    newEmp.dependentId = newDependent.dependentId;

    // Should generate 01-0002
    std::string newEmployeeID = empRepo->insertEmployee(newEmp);
    LOG_DEBUG("new Employee Id: " << newEmployeeID);

    ASSERT_FALSE(newEmployeeID.empty());
    EXPECT_EQ(empRepo->getById("01-0002")->fullName, "Bob Reyes");
    EXPECT_EQ(empRepo->getById("01-0002")->clockOutTimeStr, "18:30");
}

TEST_F(EmployeeRepoTest, GetById_ReturnsEmployeeIfExists)
{
    auto employee = empRepo->getById("00-0001");
    ASSERT_TRUE(employee.has_value());
    EXPECT_EQ(employee->fullName, "Alice Santos");
}

TEST_F(EmployeeRepoTest, GetById_ReturnsNulloptIfNotFound)
{
    auto employee = empRepo->getById("00-9999");
    EXPECT_FALSE(employee.has_value());
}

TEST_F(EmployeeRepoTest, UpdateEmployee_ModifiesExistingRecord)
{
    auto emp = empRepo->getById("00-0001");
    ASSERT_TRUE(emp.has_value());

    std::string newName = "Alice M. Santos";
    emp->fullName = newName;
    bool updated = empRepo->updateEmployee(emp.value());
    EXPECT_TRUE(updated);

    auto updatedEmp = empRepo->getById("00-0001");
    cout << updatedEmp->fullName << endl;
    ASSERT_TRUE(updatedEmp.has_value());
    EXPECT_EQ(updatedEmp->fullName, newName);
}

TEST_F(EmployeeRepoTest, DeleteEmployee_RemovesRecord)
{
    // Act
    bool deleted = empRepo->deleteEmployee("00-0001");

    // Assert
    EXPECT_TRUE(deleted);

    auto emp = empRepo->getById("00-0001");
    ASSERT_TRUE(emp.has_value());
    EXPECT_FALSE(emp->isActive);
}

TEST_F(EmployeeRepoTest, FindByName_ReturnsMatchingRecords)
{
    auto alice = empRepo->findByName("Alice Santos");
    ASSERT_FALSE(alice.empty());
    EXPECT_EQ(alice.front().fullName, "Alice Santos");

    auto bob = empRepo->findByName("bob martin");
    ASSERT_TRUE(bob.empty());
}
TEST_F(EmployeeRepoTest, GetLastEmployeeId_ReturnsMostRecent)
{
    auto lastId = empRepo->getLastEmployeeId();
    EXPECT_EQ(lastId, "05-0010");
}

TEST_F(EmployeeRepoTest, GetContact_ReturnsObjectData)
{
    std::optional<Employee> emp = empRepo->getById("00-0001");
    ASSERT_TRUE(emp.has_value());
    ASSERT_TRUE(emp.value().contactId > 0);
    std::optional<Contact> contact = contactRepo->getById(emp.value().contactId);
    ASSERT_TRUE(contact.has_value());
}

TEST_F(EmployeeRepoTest, GetDependent_ReturnsObjectData)
{
    std::optional<Employee> emp = empRepo->getById("00-0001");
    ASSERT_TRUE(emp.has_value());
    ASSERT_TRUE(emp.value().dependentId > 0);
    std::optional<Dependent> dependent = depRepo->getById(emp.value().dependentId);
    ASSERT_TRUE(dependent.has_value());
}

TEST_F(EmployeeRepoTest, DeleteAll_RemovesAllRecords)
{
    bool deleted = empRepo->deleteAll();
    EXPECT_TRUE(deleted);

    auto all = empRepo->getAll();
    EXPECT_TRUE(all.empty());
}
