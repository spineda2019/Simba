cmake_minimum_required(VERSION 3.15)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: Shim in zig
# set(CMAKE_C_COMPILER "zig")
# set(CMAKE_C_COMPILER_ARG1 "cc")
# set(CMAKE_CXX_COMPILER "zig")
# set(CMAKE_CXX_COMPILER_ARG1 "c++")

project(simba LANGUAGES CXX DESCRIPTION "The SimpleBudgetingApp")

include(FetchContent)  # For FLTK

set(FLTK_BUILD_EXAMPLES OFF CACHE BOOL "Disable FLTK examples")
set(FLTK_BUILD_TEST OFF CACHE BOOL "Disable FLTK tests")

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk.git
  GIT_TAG        release-1.4.4
  SYSTEM         # Suppress warning lib authors caused
)

# 7. Make FLTK available. This downloads and adds it as a sub-project.
FetchContent_MakeAvailable(fltk)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(simba)
