################################################################################
#                             Top Level Superbuild                             #
#                                                                              #
# Bootraps build tools (by simply wrapping zig sub commands into a single      #
# executable), Then uses these to build the actual simba project itself        #
################################################################################

cmake_minimum_required(VERSION 3.20)
project(superbuild NONE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(ExternalProject)

set(EXTERNAL_PREFIX "${CMAKE_BINARY_DIR}/zig_bootstrap")
set(EXTERNAL_INSTALL_DIR "${EXTERNAL_PREFIX}/install")

set(target "" CACHE STRING "Zig triple to target")

if(target STREQUAL "")
    message(FATAL_ERROR "-Dtarget MUST be specified")
else()
    message(STATUS "Bootstrapping for target: ${target}")
endif()

if(NOT ${target} MATCHES "^([a-zZ-Z0-9_]+)-([a-zZ-Z0-9_]+)-([a-zZ-Z0-9_.]+)$")
    message(FATAL_ERROR "Expected target=<arch>-<os>-<abi>")
endif()

set(ZIG_ARCH ${CMAKE_MATCH_1})
set(ZIG_OS ${CMAKE_MATCH_2})
set(ZIG_ABI ${CMAKE_MATCH_3})

ExternalProject_Add(ZigPassthrough
                    PREFIX ${EXTERNAL_PREFIX}
                    SOURCE_DIR "${CMAKE_SOURCE_DIR}/zig_passthrough"
                    CMAKE_ARGS
                        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
                        -Dtarget=${target}
                        -DCMAKE_BUILD_TYPE=release  # TODO: Ensure single
                                                    # config generator
                        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
                    BUILD_BYPRODUCTS
                        "${EXTERNAL_INSTALL_DIR}/bin/ar"
                        "${EXTERNAL_INSTALL_DIR}/bin/cc"
                        "${EXTERNAL_INSTALL_DIR}/bin/cxx"
                        "${EXTERNAL_INSTALL_DIR}/bin/dlltool"
                        "${EXTERNAL_INSTALL_DIR}/bin/lib"
                        "${EXTERNAL_INSTALL_DIR}/bin/objcopy"
                        "${EXTERNAL_INSTALL_DIR}/bin/ranlib"
                        "${EXTERNAL_INSTALL_DIR}/bin/rc")

message(STATUS "Superbuild: Arch: ${ZIG_ARCH}")
message(STATUS "Superbuild: OS: ${ZIG_OS}")
message(STATUS "Superbuild: ABI: ${ZIG_ABI}")

ExternalProject_Add(simba
                    SOURCE_DIR "${CMAKE_SOURCE_DIR}/simba"
                    CMAKE_ARGS
                        #                Bootstrapped Tools                #
                        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                        -Dtarget=${target}
                        -DEXTERNAL_INSTALL_DIR=${EXTERNAL_INSTALL_DIR}
                        -DZIG_ARCH=${ZIG_ARCH}
                        -DZIG_OS=${ZIG_OS}
                        -DZIG_ABI=${ZIG_ABI}
                        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_LIST_DIR}/zigtoolchain.cmake
                    DEPENDS ZigPassthrough)
