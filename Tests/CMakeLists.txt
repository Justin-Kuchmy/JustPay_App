cmake_minimum_required(VERSION 3.16)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Gather all test sources
file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp"
)

file(GLOB DB_SOURCES
    "${CMAKE_SOURCE_DIR}/Source/Repos/*.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/YearEndBenefitsService.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/LeaveBalanceService.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/PayrollService.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Helpers/PayrollCalculations.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/GovernmentRemittanceService.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/BudgetUtilService.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/TaxReconciliationService.cpp"
    "${CMAKE_SOURCE_DIR}/Source/Services/EmployeeService.cpp"
)

file(GLOB DB_HEADERS
    "${CMAKE_SOURCE_DIR}/Include/Repositories/*.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/YearEndBenefitsService.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/LeaveBalanceService.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/PayrollService.h"
    "${CMAKE_SOURCE_DIR}/Include/Helpers/PayrollCalculations.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/GovernmentRemittanceService.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/BudgetUtilService.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/TaxReconciliationService.h"
    "${CMAKE_SOURCE_DIR}/Include/Services/EmployeeService.h"
)

# Create the test executable
add_executable(JustPay_Tests
    ${TEST_SOURCES}
    ${DB_SOURCES}
    ${DB_HEADERS}
)

target_compile_features(JustPay_Tests PRIVATE cxx_std_20)

# Include directories for repositories and database
target_include_directories(JustPay_Tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/Include
)

# Link only what’s needed — skip Qt, etc.
target_link_libraries(JustPay_Tests
    PRIVATE
        gtest
        gtest_main
        sqlite3
        Qt6::Core
)

# Copy test SQL file to build folder
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tests.sql ${CMAKE_BINARY_DIR}/tests.sql COPYONLY)

# Enable GoogleTest discovery
include(GoogleTest)
gtest_discover_tests(JustPay_Tests
    TEST_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/JustTest.exe
)


# ---Google Test ---
cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
#disable internal gtest tests
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


set_target_properties(JustPay_Tests PROPERTIES
    OUTPUT_NAME "JustTest"
)
