cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(simba LANGUAGES CXX DESCRIPTION "The SimpleBudgetingApp")

# Initialize and update git submodules
execute_process(COMMAND git submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(fltk)
add_subdirectory(simba)
