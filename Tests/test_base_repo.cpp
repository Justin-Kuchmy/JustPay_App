#include <gtest/gtest.h>
#include "Repositories/BaseRepository.h"
#include <sqlite3.h>
#include <filesystem>
#include <fstream>
using std::cout;
using std::endl;

struct TestItem
{
    int id;
    std::string name;
};

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

class TestRepository : public BaseRepository
{
public:
    explicit TestRepository(sqlite3 *db) : BaseRepository(db) {}
    bool createTable() const override { return true; }

    template <typename T>
    std::vector<T> testQuery(const std::string &sql, std::function<T(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder = [](sqlite3_stmt *) {}) const
    {
        return query<T>(sql, mapper, binder);
    }

    template <typename T>
    std::optional<T> testQuerySingle(const std::string &sql, std::function<T(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder = [](sqlite3_stmt *) {}) const
    {
        return querySingle<T>(sql, mapper, binder);
    }
};

template std::vector<TestItem> BaseRepository::query(const std::string &sql, std::function<TestItem(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;
template std::optional<TestItem> BaseRepository::querySingle(const std::string &sql, std::function<TestItem(sqlite3_stmt *)> mapper, std::function<void(sqlite3_stmt *)> binder) const;

TestItem mapTestItem(sqlite3_stmt *stmt)
{
    return TestItem{
        .id = sqlite3_column_int(stmt, 0),
        .name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)),
    };
}

class BaseRepoTest : public ::testing::Test
{
protected:
    sqlite3 *db = nullptr;
    std::unique_ptr<TestRepository> repo;

    void SetUp() override
    {
        sqlite3_open("tests.db", &db);

        std::filesystem::path sqlPath = std::filesystem::path(__FILE__).parent_path() / "tests.sql";
        runSqlScript(db, sqlPath.string());

        repo = std::make_unique<TestRepository>(db);
    }

    void TearDown() override
    {
        sqlite3_close(db);
        std::remove("tests.db");
    }
};

TEST_F(BaseRepoTest, QueryReturnsMultipleRows)
{
    std::string sql = "SELECT * FROM base_repo_test_table";
    auto results = repo->testQuery<TestItem>(sql, mapTestItem);
    EXPECT_GT(results.size(), 1);
}

TEST_F(BaseRepoTest, QuerySingleReturnsRow)
{
    std::string sql = "SELECT * FROM base_repo_test_table WHERE id = 1";
    auto result = repo->testQuerySingle<TestItem>(sql, mapTestItem);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, 1);
}

TEST_F(BaseRepoTest, QuerySingleReturnsNulloptWhenNotFound)
{
    std::string sql = "SELECT * FROM base_repo_test_table WHERE id = 9999";
    auto result = repo->testQuerySingle<TestItem>(sql, mapTestItem);
    EXPECT_FALSE(result.has_value());
}

TEST_F(BaseRepoTest, BinderCallbackFiltersCorrectly)
{
    std::string sql = "SELECT * FROM base_repo_test_table WHERE id = ?";
    auto result = repo->testQuerySingle<TestItem>(sql, mapTestItem, [](sqlite3_stmt *stmt)
                                                  { sqlite3_bind_int(stmt, 1, 2); });
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->id, 2);
}