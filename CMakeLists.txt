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

ExternalProject_Add(simba
                    SOURCE_DIR "${CMAKE_SOURCE_DIR}/simba"
                    CMAKE_ARGS
                        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                        -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                        #                Bootstrapped Tools                #
                        -DCMAKE_C_COMPILER=${EXTERNAL_INSTALL_DIR}/bin/cc
                        -DCMAKE_CXX_COMPILER=${EXTERNAL_INSTALL_DIR}/bin/cxx
                        -DCMAKE_AR=${EXTERNAL_INSTALL_DIR}/bin/ar
                        -DCMAKE_RANLIB=${EXTERNAL_INSTALL_DIR}/bin/ranlib
                        -DCMAKE_OBJCOPY=${EXTERNAL_INSTALL_DIR}/bin/objcopy
                        -DCMAKE_RC_COMPILER=${EXTERNAL_INSTALL_DIR}/bin/rc
                    DEPENDS ZigPassthrough)
