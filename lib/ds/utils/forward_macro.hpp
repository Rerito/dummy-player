
#pragma once

#include <utility>

#define CPPFWD(el) std::forward<decltype(el)>(el)
