// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

// HACK: C++ has no standard way of starting child processes (for some stupid
// reason), so we must do this.
#ifdef _WIN32
#include "platform/win32.hpp"
#else
#include "platform/posix.hpp"
#endif

[[noreturn]]
int main(int argc, char **argv) {
    execute_child_zig(std::span{argv + 1, argv + argc});
}
