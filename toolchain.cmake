cmake_minimum_required(VERSION 3.15)

# -----------------------------------------------------------------------------
# Toolchain Setup
# -----------------------------------------------------------------------------

# 1. Retrieve the relative path from the environment variable 'BAR'
#    Example: export BAR="tools/linaro/bin"
set(ZIG_WRAPPER_DIR $ENV{TOOLCHAIN_DIR})

# Check if the environment variable was actually provided
if(NOT ZIG_WRAPPER_DIR)
    message(FATAL_ERROR "Environment variable 'TOOLCHAIN_DIR' is not
                         set. This should be set by the bootstrapper and point
                         to the output zig wrappers (though this can be done
                         manually)")
endif()

message(STATUS "Configuring toolchain using binaries at: ${ZIG_WRAPPER_DIR}")

# Point CMake to the specific binaries
# Use CACHE ... FORCE to ensure these values persist and override default
# system checks.

set(CMAKE_C_COMPILER "${ZIG_WRAPPER_DIR}/cc"
    CACHE
    FILEPATH
    "C Compiler"
    FORCE)
set(CMAKE_CXX_COMPILER "${ZIG_WRAPPER_DIR}/cxx"
    CACHE
    FILEPATH
    "C++ Compiler"
    FORCE)
set(CMAKE_LINKER "${CMAKE_C_COMPILER}" CACHE FILEPATH "Linker" FORCE)

# Optional: Point to other common binutils if they reside in the same directory
set(CMAKE_AR "${ZIG_WRAPPER_DIR}/ar" CACHE FILEPATH "Archiver" FORCE)
set(CMAKE_RANLIB "${ZIG_WRAPPER_DIR}/ranlib" CACHE FILEPATH "Ranlib" FORCE)
