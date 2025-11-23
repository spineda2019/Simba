// Copyright [year] <Copyright Owner>

#ifndef ZIG_PASSTHROUGH_SRC_INCLUDE_ZIG_SETUP_HPP_
#define ZIG_PASSTHROUGH_SRC_INCLUDE_ZIG_SETUP_HPP_

#include <algorithm>
#include <string_view>

namespace zig::setup {
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
}  // namespace zig::setup
#endif  // ZIG_PASSTHROUGH_SRC_INCLUDE_ZIG_SETUP_HPP_
