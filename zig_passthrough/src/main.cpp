// Copyright 2025 Sebastian Pineda (spineda.wpi.alum@gmail.com)

// HACK: C++ has no standard way of starting child processes (for some stupid
// reason), so we must do this.
#include <iostream>
#include <span>
#include <vector>
#ifdef _WIN32
#include "platform/win32.hpp"
#else
#include "platform/posix.hpp"
#endif

#include "include/zig_setup.hpp"

namespace {
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
}  // namespace

[[noreturn]]
int main(int argc, char **argv) {
    std::span<char *> args{argv + 1, argv + argc};
    std::vector<char *> built_args{};
    built_args.reserve(args.size() + 3);

    std::array<char, 4> zig_name{'z', 'i', 'g', 0};
    std::array<char, 8> target_name{'-', 't', 'a', 'r', 'g', 'e', 't', 0};
    constexpr zig::setup::ComptimeString zig_triple_str{ZIG_TRIPLE};
    std::array<char, decltype(zig_triple_str)::len> zig_triple{};
    for (std::size_t idx{0}; idx < zig_triple.size(); ++idx) {
        zig_triple[idx] = ZIG_TRIPLE[idx];
    }
    constexpr zig::setup::ComptimeString subcommand_str{SUBCOMMAND};
    std::array<char, decltype(subcommand_str)::len> subcommand{};
    for (std::size_t idx{0}; idx < subcommand.size(); ++idx) {
        subcommand[idx] = SUBCOMMAND[idx];
    }

    built_args.push_back(zig_name.begin());
    built_args.push_back(subcommand.begin());
    if constexpr (inject_target_flag()) {
        built_args.push_back(target_name.begin());
        built_args.push_back(zig_triple.begin());
    }
    built_args.append_range(args);
    built_args.push_back(nullptr);

    execute_child_zig(built_args);
}
