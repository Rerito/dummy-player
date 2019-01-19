
#pragma once

#include <random>
#include <algorithm>

#include "utils/forward_macro.hpp"

namespace dp {

template <typename URBG = std::mt19937_64>
class playlist_shuffler {
    URBG g_;
public:

    playlist_shuffler(playlist_shuffler const&) = default;
    playlist_shuffler& operator=(playlist_shuffler const&) = default;

    playlist_shuffler(playlist_shuffler&&) = default;
    playlist_shuffler& operator=(playlist_shuffler&&) = default;

    template <typename... Args>
    playlist_shuffler(Args&&... args) : g_(CPPFWD(args)...) {}

    template <typename RandomIt>
    void shuffle(RandomIt b, RandomIt e) const {
        std::random_shuffle(b, e, g_);
    }
};

} // namespace dp
