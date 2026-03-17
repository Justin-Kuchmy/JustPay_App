#include <gtest/gtest.h>
#include "Repositories/DependentRepository.h"
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

class DependentRepoTest : public ::testing::Test {
protected:
    sqlite3* db = nullptr;
    std::unique_ptr<DependentRepository> repo;

    void SetUp() override {
        const char* dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<DependentRepository>(db);
    }

    void TearDown() override {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};


TEST_F(DependentRepoTest, GetById_ReturnsDependentIfExists)
{
    std::optional<Dependent> Dependent = repo->getById(1);
    ASSERT_TRUE(Dependent.has_value());
    EXPECT_EQ(Dependent.value().name, "Juan Santos"); 
}

TEST_F(DependentRepoTest, GetById_ReturnsNulloptIfNotFound)
{
    std::optional<Dependent> Dependent = repo->getById(99);
    EXPECT_FALSE(Dependent.has_value()) << "Expected no Dependent with ID=99.";
}

TEST_F(DependentRepoTest, InsertDependent_AddsNewRecord) 
{
    Dependent newDependent;
    newDependent.name = "dep_Name";
    newDependent.relation = "dep_Relation";
    newDependent.birthday = Date::fromString("2001-01-01");


    int newDependentId = repo->insertDependent(newDependent);
    EXPECT_GT(newDependentId, 0) << "Expected a positive DependentId after insertion";
}

TEST_F(DependentRepoTest, UpdateDependent_ModifiesExistingRecord) 
{
    std::optional<Dependent> Dependent = repo->getById(1);
    ASSERT_TRUE(Dependent.has_value());
    Dependent.value().name = "Do Santos";
    ASSERT_TRUE(repo->updateDependent(Dependent.value()));
    EXPECT_EQ(repo->getById(1).value().name, "Do Santos");
}

TEST_F(DependentRepoTest, DeleteDependent_RemovesRecord) 
{
    std::optional<Dependent> Dependent = repo->getById(1);
    ASSERT_TRUE(Dependent.has_value());
    Dependent.value().name = "Do Santos";
    ASSERT_TRUE(repo->deleteDependent(Dependent.value().dependentId));
    ASSERT_FALSE(repo->getById(1));
}