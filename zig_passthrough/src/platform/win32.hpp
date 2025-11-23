#ifndef ZIG_PASSTHROUGH_SRC_PLATFORM_WIN32_HPP_
#define ZIG_PASSTHROUGH_SRC_PLATFORM_WIN32_HPP_
#include <process.h>  // Required for _spawnvp and _P_WAIT

#include <iostream>
#include <ostream>
#include <span>
#include <vector>

[[noreturn]]
void execute_child_process(const std::vector<char*>& built_args) {
    // _spawnvp executes the command, searches the PATH (due to the 'p' suffix),
    // and takes the arguments array (due to the 'v' suffix).
    // _P_WAIT instructs the parent process to wait until the child process
    // terminates. The executable name is taken dynamically from the first
    // argument (command).
    std::vector<char*> args{built_args.begin() + 1, built_args.end()};
    intptr_t status = _spawnvp(_P_WAIT, built_args[0], args.data());

    if (status == -1) {
        // Error launching the process (e.g., command not found in PATH)
        std::cerr << "Error: could not launch zig" << std::endl;

        // Use the standard POSIX exit code for command not found
        constexpr int COMMAND_NOT_FOUND{127};
        std::exit(COMMAND_NOT_FOUND);
    }

    std::exit(static_cast<int>(status));
}
#endif  // ZIG_PASSTHROUGH_SRC_PLATFORM_WIN32_HPP_
