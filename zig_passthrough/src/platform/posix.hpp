#ifndef ZIG_PASSTHROUGH_SRC_PLATFORM_POSIX_HPP_
#define ZIG_PASSTHROUGH_SRC_PLATFORM_POSIX_HPP_

#include <fcntl.h>     // open, O_WRONLY, O_CREAT
#include <sys/wait.h>  // waitpid
#include <unistd.h>    // fork, execvp

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <span>
#include <string_view>
#include <vector>

template <std::size_t Length>
struct ComptimeString final {
    /// Cannot be marked as explicit, as this is intended to be used as a
    /// template argument in ArgumentParser, so this allows implicit conversions
    /// (e.g. ArgumentParser<"FooProgram">)
    consteval ComptimeString(const char (&name)[Length]) {
        // don't include null terminator. That's C crap.
        std::copy(name, name + Length - 1, str_.begin());
    }

    constexpr bool operator==(const ComptimeString &) const = default;
    constexpr auto operator<=>(const ComptimeString &) const = default;

    consteval std::string_view PrintableView() const {
        return std::string_view{str_.begin(), str_.size()};
    }

    std::array<char, Length - 1> str_;
    static inline constexpr std::size_t len{Length};
};

template <char Front, char... Rest>
struct CountChars final {
    static inline constexpr std::size_t value{1 + CountChars<Rest...>::value};
};

consteval bool inject_target_flag() {
    constexpr std::array<std::string_view, 2> NEEDED{std::string_view{"cc"},
                                                     std::string_view{"c++"}};

    for (std::string_view candidate : NEEDED) {
        if (candidate == SUBCOMMAND) {
            return true;
        }
    }

    return false;
}

[[noreturn]]
void execute_child_zig(const std::span<char *> args) {
    std::array<char, 4> zig_name{'z', 'i', 'g', 0};
    std::array<char, 8> target_name{'-', 't', 'a', 'r', 'g', 'e', 't', 0};
    constexpr ComptimeString zig_triple_str{ZIG_TRIPLE};
    std::array<char, decltype(zig_triple_str)::len> zig_triple{};
    for (std::size_t idx{0}; idx < zig_triple.size(); ++idx) {
        zig_triple[idx] = ZIG_TRIPLE[idx];
    }
    constexpr ComptimeString subcommand_str{SUBCOMMAND};
    std::array<char, decltype(subcommand_str)::len> subcommand{};
    for (std::size_t idx{0}; idx < subcommand.size(); ++idx) {
        subcommand[idx] = SUBCOMMAND[idx];
    }

    std::vector<char *> built_args{};
    built_args.reserve(args.size() + 3);
    built_args.push_back(zig_name.begin());
    built_args.push_back(subcommand.begin());
    if constexpr (inject_target_flag()) {
        built_args.push_back(target_name.begin());
        built_args.push_back(zig_triple.begin());
    }
    built_args.append_range(args);

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
