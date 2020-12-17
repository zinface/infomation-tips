cmake_minimum_required(VERSION 3.0.0)

project(infomation-tips VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core DBus Widgets)


add_executable(${PROJECT_NAME}
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::DBus Qt5::Widgets)



# Archive build control
set(BUILD_TYPE "${BUILD_TYPE}")
if(BUILD_TYPE STREQUAL "TGZ")
    include(cmake/BuildSourceTgz.cmake)
else()
    include(cmake/DebGenerater.cmake)
endif()
