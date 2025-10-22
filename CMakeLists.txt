cmake_minimum_required(VERSION 3.16)

project(JustPay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------
# Qt Setup
# ---------------------------------------------------------
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick QuickWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Include/UI/Generated")

# ---------------------------------------------------------
# Include directories
# ---------------------------------------------------------
include_directories(
    ${CMAKE_SOURCE_DIR}/Include
)

# ---------------------------------------------------------
# Shared source files for all builds
# ---------------------------------------------------------
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/main_qt.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/UI/*.cpp"
    "${CMAKE_SOURCE_DIR}/UI/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/Include/*.h"
    "${CMAKE_SOURCE_DIR}/Include/**/*.h"
)

# Collect UI files
file(GLOB_RECURSE UI_FILES
    "${CMAKE_SOURCE_DIR}/Include/UI/*.ui"
    "${CMAKE_SOURCE_DIR}/Include/UI/Components/*.ui"
)

#---------------------------------------------------------
#=== MAIN APP TARGET ===
#----------------------------------------------------------

add_executable(JustPay
    ${CMAKE_SOURCE_DIR}/main_qt.cpp
    ${APP_SOURCES}
    ${UI_FILES}
)

set_target_properties(JustPay PROPERTIES
    OUTPUT_NAME "JustPay.exe"
    AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/Include/UI/Components"
)
    
target_link_libraries(JustPay
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickWidgets
        sqlite3
)

# ---------------------------------------------------------
# === TEST APP TARGET ===
# ---------------------------------------------------------
# Filter out main_qt.cpp so the test build doesn't include two mains
set(TEST_SOURCES ${APP_SOURCES})
list(FILTER TEST_SOURCES EXCLUDE REGEX "main_qt\\.cpp$")

add_executable(JustPay_Tests
    ${CMAKE_SOURCE_DIR}/tests/TestRunner.cpp
    ${TEST_SOURCES}
    ${UI_FILES}
)

set_target_properties(JustPay_Tests PROPERTIES
    OUTPUT_NAME "JustPay_TestRunner.exe"
    AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/Include/UI/Components"
)

target_link_libraries(JustPay_Tests
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickWidgets
        sqlite3
)

# ---------------------------------------------------------
# Optional: group targets in IDEs like Qt Creator or VS Code
# ---------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(JustPay PROPERTIES FOLDER "Apps")
set_target_properties(JustPay_Tests PROPERTIES FOLDER "Tests")