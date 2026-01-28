    cmake_minimum_required(VERSION 3.16)

    project(JustPay LANGUAGES CXX)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    # ---------------------------------------------------------
    # Qt Setup
    # ---------------------------------------------------------
    find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Quick QuickWidgets Sql PrintSupport)

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -pedantic-errors")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -Wall")

    set(WARNING_EFFC "-Weffc++")
    set(WARNING_CONVERSION "-Wconversion -Wsign-conversion")
    set(WARNING_EXTRA "-Wall -Wextra -pedantic-errors")

    set(FOCUS_WARNING ${WARNING_EFFC})
    set(FOCUS_WARNING ${WARNING_CONVERSION})
    set(FOCUS_WARNING ${WARNING_EXTRA})



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
    "${CMAKE_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_SOURCE_DIR}/Source/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/Include/*.h"
    "${CMAKE_SOURCE_DIR}/Include/**/*.h"
    "${CMAKE_SOURCE_DIR}/UI/*.cpp"
    "${CMAKE_SOURCE_DIR}/UI/**/*.cpp"
)

# Collect UI files
file(GLOB_RECURSE UI_FILES
    "${CMAKE_SOURCE_DIR}/UI/*.ui"
)

# ---------------------------------------------------------
# Include resource file if it exists
# ---------------------------------------------------------
set(RESOURCE_FILE "${CMAKE_SOURCE_DIR}/Resources/resources.qrc")
if(EXISTS ${RESOURCE_FILE})
    list(APPEND APP_SOURCES ${RESOURCE_FILE})
endif()

#---------------------------------------------------------
#=== MAIN APP TARGET ===
#----------------------------------------------------------
add_executable(JustPay
    ${APP_SOURCES}
    ${UI_FILES}
)

target_include_directories(JustPay PRIVATE
${CMAKE_SOURCE_DIR}/Include
)



set(CMAKE_AUTOUIC_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Generated")


set_target_properties(JustPay PROPERTIES AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/UI/Dialogs")

    

# ---------------------------------------------------------
# Link Qt and SQLite libraries
# ---------------------------------------------------------
target_link_libraries(JustPay
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickWidgets
        Qt6::Sql 
        Qt6::PrintSupport
        sqlite3
)


set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_target_properties(JustPay PROPERTIES FOLDER "Apps")

# ---------------------------------------------------------
# === TESTS SUBDIRECTORY ===
# ---------------------------------------------------------
if(EXISTS "${CMAKE_SOURCE_DIR}/Tests/CMakeLists.txt")
    add_subdirectory(Tests)
endif()


