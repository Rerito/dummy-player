
#pragma once

#include <string>

#include "utils/forward_macro.hpp"

template <typename T>
struct type_conversion {
    static std::string to_string(T const& t) {
        return std::string(t);
    }

    static T from_string(std::string const& s) {
        return T(s);
    }
};

template <>
struct type_conversion<std::string> {
    template <typename Str>
    static decltype(auto) to_string(Str&& str) {
        return CPPFWD(str);
    }
    
    template <typename Str>
    static decltype(auto) from_string(Str&& str) {
        return CPPFWD(str);
    }
};

template <>
struct type_conversion<int> {
    static int from_string(std::string const& s) {
        return std::stoi(s);
    }

    static std::string to_string(int i) {
        return std::to_string(i);
    }
};
