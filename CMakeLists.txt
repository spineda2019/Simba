# Project settings
cmake_minimum_required( VERSION 3.15 )

project(simba
        VERSION 1.0
        DESCRIPTION "Simple Budgeting App"
        LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_STRIP_RELEASE TRUE)
set(CMAKE_STRIP_MINSIZEREL TRUE)

# Fetch the package
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

if(NOT Qt6_FOUND)
    message(FATAL "Qt 6 not found...")
else()
    message(STATUS "Qt 6.4.2 found in the system.")
endif()

add_subdirectory(simba)
