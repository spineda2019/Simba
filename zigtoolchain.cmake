include_guard()

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    message(FATAL_ERROR "Visual Studio generator not supported, use: cmake -G Ninja")
endif()

if(ZIG_OS STREQUAL "linux")
    set(CMAKE_SYSTEM_NAME "Linux")
elseif(ZIG_OS STREQUAL "windows")
    set(CMAKE_SYSTEM_NAME "Windows")
elseif(ZIG_OS STREQUAL "macos")
    set(CMAKE_SYSTEM_NAME "Darwin")
elseif(ZIG_OS STREQUAL "freestanding")
    set(CMAKE_SYSTEM_NAME "Generic")
elseif(ZIG_OS STREQUAL "uefi")
    set(CMAKE_SYSTEM_NAME "UEFI")
    # Fix compiler detection (lld-link:
    # error: <root>: undefined symbol: EfiMain)
    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
elseif(ZIG_OS STREQUAL "wasi")
    set(CMAKE_SYSTEM_NAME "WASI")
elseif(ZIG_OS STREQUAL "emscripten")
    set(CMAKE_SYSTEM_NAME "Emscripten")
    # else()
    # message(FATAL_ERROR "DEBUG OS: ${ZIG_OS}")
endif()

set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR ${ZIG_ARCH})

# Work around a bug in clangd where 'c++' is reordered on the command line
set(CMAKE_C_COMPILER "${EXTERNAL_INSTALL_DIR}/bin/cc")
set(CMAKE_CXX_COMPILER "${EXTERNAL_INSTALL_DIR}/bin/cxx")

# This is working (thanks to Simon for finding this trick)
set(CMAKE_AR "${EXTERNAL_INSTALL_DIR}/bin/ar")
set(CMAKE_RANLIB "${EXTERNAL_INSTALL_DIR}/bin/ranlib")
set(CMAKE_RC_COMPILER "${EXTERNAL_INSTALL_DIR}/bin/rc")

# Add custom UEFI platform to module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
