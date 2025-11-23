#ifndef ZIG_PASSTHROUGH_SRC_PLATFORM_POSIX_HPP_
#define ZIG_PASSTHROUGH_SRC_PLATFORM_POSIX_HPP_

#include <fcntl.h>     // open, O_WRONLY, O_CREAT
#include <sys/wait.h>  // waitpid
#include <unistd.h>    // fork, execvp

#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <span>
#include <vector>

[[noreturn]]
void execute_child_zig(const std::vector<char*>& built_args) {
    pid_t pid{fork()};
    if (pid < 0) {
        std::cerr << "Error: Fork failed.";
        std::exit(pid);
    } else if (pid == 0) {
        // Execute the command
        execvp("zig", built_args.data());

        constexpr int COMMAND_NOT_FOUND{127};
        std::exit(COMMAND_NOT_FOUND);
    } else {
        // --- Parent Process ---
        waitpid(pid, nullptr, 0);
        std::exit(0);
    }
}
#endif  // ZIG_PASSTHROUGH_SRC_PLATFORM_POSIX_HPP_
