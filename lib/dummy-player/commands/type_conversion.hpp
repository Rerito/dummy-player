
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

template <typename T>
struct to_string_s {
    template <typename U>
    std::string operator()(U&& el) const {
        using std::to_string;
        return to_string(CPPFWD(el));
    }
};

template <>
struct to_string_s<std::string> {
    template <typename T>
    std::string operator()(T&& str) const {
        return CPPFWD(str);
    }
};
