
#pragma once

#include <string>
#include <algorithm>
#include <stdexcept>
#include <array>

#include "commands/type_conversion_fwd.hpp"

namespace dp {

enum class ShuffleMode {
    RANDOM=0,
    NATIVE
};

constexpr std::array<char const*, 2u> shuffle_mode_strings = { "RANDOM", "NATIVE" };

} // namespace dp

template <>
struct from_string<dp::ShuffleMode> {
    dp::ShuffleMode operator()(std::string const& s) const {
        auto tgt = std::find_if(
            begin(dp::shuffle_mode_strings),
            end(dp::shuffle_mode_strings),
            [&](auto const& el) { return s == el; }
        );
        if (end(dp::shuffle_mode_strings) == tgt) {
            throw std::invalid_argument("invalid string: " + s);
        }
        return static_cast<dp::ShuffleMode>(tgt - begin(dp::shuffle_mode_strings));
    }
};

template <>
struct to_string_s<dp::ShuffleMode> {
    std::string operator()(dp::ShuffleMode mode) const {
        return dp::shuffle_mode_strings[static_cast<size_t>(mode)];
    }
};
