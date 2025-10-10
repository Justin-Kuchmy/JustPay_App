cmake_minimum_required(VERSION 3.16)

project(JustPay LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS QuickWidgets)

include_directories(${CMAKE_SOURCE_DIR}/include)


set(CMAKE_AUTOMOC ON)   # <-- this is key
set(CMAKE_AUTOUIC ON)   # (useful later for .ui files)
set(CMAKE_AUTORCC ON)   # (useful later for resources)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/main_qt.cpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/UI/*.cpp"
    "${CMAKE_SOURCE_DIR}/UI/**/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/**/*.h"
    

)

add_executable(JustPay ${SOURCES})  
    
set_target_properties(JustPay PROPERTIES OUTPUT_NAME "JustPay.exe")
    
# Link SQLite
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::Widgets
        sqlite3
)
