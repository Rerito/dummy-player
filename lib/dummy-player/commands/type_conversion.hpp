
#pragma once

#include <string>

#include "utils/forward_macro.hpp"

template <typename T>
struct from_string {
    T operator()(std::string const& s) const {
        return T(s);
    }
};

template <>
struct from_string<int> {
    int operator()(std::string const& s) const {
        return std::stoi(s);
    }
};



