
#pragma once

#include <string>
#include <algorithm>
#include <stdexcept>
#include <array>

#include "commands/type_conversion_fwd.hpp"

namespace dp {

enum class RepeatMode {
    NO_REPEAT = 0,
    REPEAT_1,
    REPEAT_ALL,
    END_REPEAT_MODE
};

constexpr std::array<char const*, 3u> repeat_mode_strings = { "NONE", "ONE", "ALL" };

} // namespace dp

template <>
struct from_string<dp::RepeatMode> {
    dp::RepeatMode operator()(std::string const& s) const {
        auto tgt = std::find_if(
            begin(dp::repeat_mode_strings),
            end(dp::repeat_mode_strings),
            [&](auto const& el) { return s == el; }
        );
        if (end(dp::repeat_mode_strings) == tgt) {
            throw std::invalid_argument("invalid string: " + s);
        }
        return static_cast<dp::RepeatMode>(tgt - begin(dp::repeat_mode_strings));
    }
};

template <>
struct to_string_s<dp::RepeatMode> {
    std::string operator()(dp::RepeatMode mode) const {
        if (dp::RepeatMode::END_REPEAT_MODE == mode) {
            throw std::invalid_argument("invalid argument mode: END_REPEAT_MODE is reserved for static usage");
        }
        return dp::repeat_mode_strings[static_cast<size_t>(mode)];
    }
};
