#include <gtest/gtest.h>
#include "Repositories/EmergencyContactRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"

static void runSqlScript(sqlite3* db, const std::string& sqlFilePath)
{
    std::ifstream file(sqlFilePath);
    ASSERT_TRUE(file.is_open()) << "Failed to open SQL file: " << sqlFilePath;
    std::string sql((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    char* errMsg = nullptr;
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
     if (rc != SQLITE_OK) {
        std::cerr << "SQLite error running script: " << sqlFilePath << "\n";
        std::cerr << "   → Error message: " << (errMsg ? errMsg : "unknown") << "\n";

        std::cerr << "   → Script snippet near failure:\n";
        size_t pos = sql.find_last_of(';'); 
        if (pos != std::string::npos) {
            std::string snippet = sql.substr(pos > 200 ? pos - 200 : 0, 200);
            std::cerr << snippet << "\n";
        }

        ASSERT_EQ(rc, SQLITE_OK) << "SQL error: " << (errMsg ? errMsg : "unknown");
    }
    if (errMsg) sqlite3_free(errMsg);
};

class ContactRepoTest : public ::testing::Test {
protected:
    sqlite3* db = nullptr;
    std::unique_ptr<EmergencyContactRepository> repo;

    void SetUp() override {
        const char* dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<EmergencyContactRepository>(db);
    }

    void TearDown() override {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(ContactRepoTest, GetById_ReturnsContactIfExists)
{
    std::optional<Contact> contact = repo->getById(1);
    ASSERT_TRUE(contact.has_value());
    EXPECT_EQ(contact.value().name, "Juan Santos");
}

TEST_F(ContactRepoTest, GetById_ReturnsNulloptIfNotFound)
{
    std::optional<Contact> contact = repo->getById(99);
    EXPECT_FALSE(contact.has_value()) << "Expected no contact with ID=99.";
}

TEST_F(ContactRepoTest, InsertContact_AddsNewRecord) 
{    
    Contact newContact;
    newContact.name = "cont_Name";
    newContact.relation = "cont_Relation";
    newContact.contactNo = "cont_Number";
    newContact.address = "cont_Addr";

    int newContactId = repo->insertContact(newContact);
    EXPECT_GT(newContactId, 0) << "Expected a positive contactId after insertion";
    
   
}

TEST_F(ContactRepoTest, UpdateContact_ModifiesExistingRecord) {
    std::optional<Contact> contact = repo->getById(1);
    ASSERT_TRUE(contact.has_value());
    contact.value().name = "Do Santos";
    ASSERT_TRUE(repo->updateContact(contact.value()));
    EXPECT_EQ(repo->getById(1).value().name, "Do Santos");
}

TEST_F(ContactRepoTest, DeleteContact_RemovesRecord) {
    std::optional<Contact> contact = repo->getById(1);
    ASSERT_TRUE(contact.has_value());
    ASSERT_TRUE(repo->deleteContact(contact.value().contactId));
    ASSERT_FALSE(repo->getById(1).has_value());
}
