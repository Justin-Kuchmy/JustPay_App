#include <gtest/gtest.h>
#include "Repositories/AttendanceLogRepository.h"
#include <sqlite3.h>
#include <memory>
#include <cstdio>
#include <filesystem>
#include <fstream>
#define DEBUG_LOGS
#include "Utils/Log.h"

static void runSqlScript(sqlite3 *db, const std::string &sqlFilePath)
{
    std::ifstream file(sqlFilePath);
    ASSERT_TRUE(file.is_open()) << "Failed to open SQL file: " << sqlFilePath;
    std::string sql((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    char *errMsg = nullptr;
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQLite error running script: " << sqlFilePath << "\n";
        std::cerr << "   → Error message: " << (errMsg ? errMsg : "unknown") << "\n";

        std::cerr << "   → Script snippet near failure:\n";
        size_t pos = sql.find_last_of(';');
        if (pos != std::string::npos)
        {
            std::string snippet = sql.substr(pos > 200 ? pos - 200 : 0, 200);
            std::cerr << snippet << "\n";
        }

        ASSERT_EQ(rc, SQLITE_OK) << "SQL error: " << (errMsg ? errMsg : "unknown");
    }
    if (errMsg)
        sqlite3_free(errMsg);
};

class AttendanceRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<AttendanceLogRepository> repo;

    void SetUp() override
    {
        const char *dbName = "tests.db";
        sqlite3_open(dbName, &db);

        // Build absolute path to the SQL file
        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<AttendanceLogRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(AttendanceRepoTest, GetById_ReturnsAttendanceLogIfExists)
{
    std::optional<AttendanceLog> log = repo->getById(1);
    ASSERT_TRUE(log.has_value());
    EXPECT_EQ(log.value().employeeId, "00-0001");
}

TEST_F(AttendanceRepoTest, GetById_ReturnsNulloptIfNotFound)
{
    std::optional<AttendanceLog> AttendanceLog = repo->getById(99);
    EXPECT_FALSE(AttendanceLog.has_value()) << "Expected no AttendanceLog with ID=99.";
}
TEST_F(AttendanceRepoTest, InsertAttendanceLog_AddsNewRecord)
{
    AttendanceLog al;
    al.employeeId = "00-0001";
    al.logDate = Date(2025, 11, 26);
    al.lateByMinute = 5.0f;
    al.overTimeByMinute = 45;
    al.underTimeByMinute = 0.0;
    al.isAbsent = false;
    al.notes = "Test attendance log entry";
    al.overtimeJson = "some json format";

    int alID = repo->insertAttendanceLog(al);
    EXPECT_GT(alID, 0) << "Expected a positive AttendanceLogId after insertion";
}

TEST_F(AttendanceRepoTest, GetAllById_ReturnsAttendanceLogIfExists)
{
    std::vector<AttendanceLog> al = repo->getAllById("00-0001");
    ASSERT_TRUE(al.size());
    EXPECT_EQ(al.size(), 2);
}

TEST_F(AttendanceRepoTest, GetAll_ReturnsAttendanceLogIfExists)
{
    std::vector<AttendanceLog> al = repo->getAll();
    ASSERT_TRUE(al.size());
    EXPECT_EQ(al.size(), 20);
}

TEST_F(AttendanceRepoTest, UpdateAttendanceLog_ModifiesExistingRecord)
{
    std::optional<AttendanceLog> attendanceLog = repo->getById(1);
    ASSERT_TRUE(attendanceLog.has_value());
    AttendanceLog logObj = attendanceLog.value();
    logObj.overTimeByMinute = 60;
    bool updatedBool = repo->updateAttendanceLog(logObj);
    ASSERT_TRUE(updatedBool);
    auto updatedObj = repo->getById(1);
    ASSERT_TRUE(updatedObj.has_value());
    EXPECT_EQ(updatedObj.value().overTimeByMinute, 60);
}
TEST_F(AttendanceRepoTest, DeleteAttendanceLog_RemovesRecord)
{
    std::optional<AttendanceLog> AttendanceLog = repo->getById(1);
    ASSERT_TRUE(AttendanceLog.has_value());
    ASSERT_TRUE(repo->deleteAttendanceLog(AttendanceLog.value().logId));
    ASSERT_FALSE(repo->getById(1).has_value());
}