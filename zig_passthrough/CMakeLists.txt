cmake_minimum_required(VERSION 3.20)  # needed for C++23
project(ZigPassthrough)

add_executable(ar      "src/main.cpp")
add_executable(cc      "src/main.cpp")
add_executable(cxx     "src/main.cpp")
add_executable(dlltool "src/main.cpp")
add_executable(lib     "src/main.cpp")
add_executable(objcopy "src/main.cpp")
add_executable(ranlib  "src/main.cpp")
add_executable(rc      "src/main.cpp")

# Pass the CMake variable to the C++ code as a macro
# This creates a definition like: #define GREETING_NAME "World"
target_compile_definitions(ar PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(ar PRIVATE SUBCOMMAND="ar")
target_compile_features(ar PRIVATE cxx_std_23)

target_compile_definitions(cc PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(cc PRIVATE SUBCOMMAND="cc")
target_compile_features(cc PRIVATE cxx_std_23)

target_compile_definitions(cxx PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(cxx PRIVATE SUBCOMMAND="c++")
target_compile_features(cxx PRIVATE cxx_std_23)

target_compile_definitions(dlltool PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(dlltool PRIVATE SUBCOMMAND="dlltool")
target_compile_features(dlltool PRIVATE cxx_std_23)

target_compile_definitions(lib PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(lib PRIVATE SUBCOMMAND="lib")
target_compile_features(lib PRIVATE cxx_std_23)

target_compile_definitions(objcopy PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(objcopy PRIVATE SUBCOMMAND="objcopy")
target_compile_features(objcopy PRIVATE cxx_std_23)

target_compile_definitions(ranlib PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(ranlib PRIVATE SUBCOMMAND="ranlib")
target_compile_features(ranlib PRIVATE cxx_std_23)

target_compile_definitions(rc PRIVATE ZIG_TRIPLE="${target}")
target_compile_definitions(rc PRIVATE SUBCOMMAND="rc")
target_compile_features(rc PRIVATE cxx_std_23)

install(TARGETS ar cc cxx dlltool lib objcopy ranlib rc)
