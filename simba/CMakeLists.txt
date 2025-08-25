add_executable(simba main.cpp)

target_compile_options(simba PRIVATE -Wall
                                     -Wextra
                                     -Wpedantic
                                     -Wshadow
                                     -Wconversion
                                     -Werror)
target_link_libraries(simba PRIVATE fltk::fltk)
