cmake_minimum_required(VERSION 3.20)  # needed for C++23
project(simba LANGUAGES CXX DESCRIPTION "The SimpleBudgetingApp")

set(CMAKE_STRIP_RELEASE TRUE)
set(CMAKE_STRIP_MINSIZEREL TRUE)
include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    # Enable LTO globally for Release builds
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)

    # And small builds
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL TRUE)
else()
    message(WARNING "LTO is not supported: ${output}")
endif()

include(FetchContent)  # For FLTK
add_executable(simba main.cpp
                     MainWindow.cpp
                     filesystem.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FLTK_BUILD_EXAMPLES OFF CACHE BOOL "Disable FLTK examples")
set(FLTK_BUILD_TEST OFF CACHE BOOL "Disable FLTK tests")

FetchContent_Declare(
  fltk
  GIT_REPOSITORY https://github.com/fltk/fltk.git
  GIT_TAG        release-1.4.4
  SYSTEM         # Suppress warning lib authors caused
)

set(FLTK_BUILD_FLUID OFF CACHE BOOL "Disable FLUID" FORCE)
set(FLTK_BUILD_TEST OFF CACHE BOOL "Disable Tests" FORCE)
set(FLTK_BUILD_EXAMPLES OFF CACHE BOOL "Disable Examples" FORCE)
FetchContent_MakeAvailable(fltk)

target_compile_options(simba PRIVATE -Wall
                                     -Wextra
                                     -Wpedantic
                                     -Wshadow
                                     -Wconversion
                                     -Werror)
target_compile_features(simba PUBLIC cxx_std_23)
target_include_directories(simba PRIVATE "${FLTK_SOURCE_DIR}/include")
target_include_directories(simba PRIVATE "${CMAKE_SOURCE_DIR}")
target_link_libraries(simba PRIVATE fltk::fltk)
