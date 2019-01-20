
#pragma once

#include <array>
#include <utility> // For index_sequence related things
#include <optional>

#include "meta/function.hpp"

#include "repeat_mode.hpp"
#include "access.hpp"

namespace dp {

namespace detail {


template <RepeatMode>
struct track_navigation_fns;


template <>
struct track_navigation_fns<RepeatMode::REPEAT_1> {
    template <typename It>
    static It get_next_track(It cur, It, It) {
        return cur;
    }

    template <typename It>
    static It get_prev_track(It cur, It, It) {
        return cur;
    }
};

template <>
struct track_navigation_fns<RepeatMode::REPEAT_ALL> {
    template <typename It>
    static It get_next_track(It cur, It first, It eot) {
        if (++cur == eot) {
            return first;
        } else {
            return cur;
        }
    }

    template <typename It>
    static It get_prev_track(It cur, It first, It eot) {
        if (cur == first) {
            return --eot;
        } else {
            return --cur;
        }
    }
};

template <>
struct track_navigation_fns<RepeatMode::NO_REPEAT> {
    template <typename It>
    static It get_next_track(It cur, It, It) {
        return ++cur;
    }

    template <typename It>
    static It get_prev_track(It cur, It first, It) {
        if (cur != first) {
            --cur;
        }
        return cur;
    }
};

template <typename It, size_t... Is>
constexpr auto make_next_track_fn_arr(std::index_sequence<Is...>) -> std::array<meta::function_ptr<It(It, It, It)>, static_cast<size_t>(RepeatMode::END_REPEAT_MODE)> {
    return { &track_navigation_fns<static_cast<RepeatMode>(Is)>::template get_next_track<It>... };
}

template <typename It>
constexpr auto make_next_track_fn_arr() {
    return make_next_track_fn_arr<It>(std::make_index_sequence<static_cast<size_t>(RepeatMode::END_REPEAT_MODE)> {});
}

template <typename It, size_t... Is>
constexpr auto make_prev_track_fn_arr(std::index_sequence<Is...>) -> std::array<meta::function_ptr<It(It, It, It)>, static_cast<size_t>(RepeatMode::END_REPEAT_MODE)> {
    return { &track_navigation_fns<static_cast<RepeatMode>(Is)>::template get_prev_track<It>... };
}

template <typename It>
constexpr auto make_prev_track_fn_arr() {
    return make_prev_track_fn_arr<It>(std::make_index_sequence<static_cast<size_t>(RepeatMode::END_REPEAT_MODE)> {});
}
} // namespace detail

// Template Magic Switch implementation for the next_track feature
template <typename It>
It get_next_track(RepeatMode mode, It current_track, It first_track, It end_of_tracks) {
    static constexpr auto next_fns = detail::make_next_track_fn_arr<It>();
    if (RepeatMode::END_REPEAT_MODE == mode) {
        throw std::invalid_argument("mode");
    }
    return next_fns[static_cast<size_t>(mode)](current_track, first_track, end_of_tracks);
}

// Template Magic Switch implementation for the prev_track feature
template <typename It>
It get_prev_track(RepeatMode mode, It current_track, It first_track, It end_of_tracks) {
    static constexpr auto prev_fns = detail::make_prev_track_fn_arr<It>();
    if (RepeatMode::END_REPEAT_MODE == mode) {
        throw std::invalid_argument("mode");
    }
    return prev_fns[static_cast<size_t>(mode)](current_track, first_track, end_of_tracks);
}

template <typename MusicCache>
std::optional<typename MusicCache::playlist_type::value_type> next_track(RepeatMode mode, MusicCache& mcache) {
    auto& pl = access::get_playlist(mcache);
    auto cur_track_it = access::get_current_track(mcache);
    auto nxt_it = get_next_track(mode, cur_track_it, begin(pl), end(pl));
    access::set_current_track(mcache, nxt_it);
    if (nxt_it == end(pl)) {
        return {};
    } else {
        return *nxt_it;
    }
}

template <typename MusicCache>
std::optional<typename MusicCache::playlist_type::value_type> previous_track(RepeatMode mode, MusicCache& mcache) {
    auto& pl = access::get_playlist(mcache);
    auto cur_track_it = access::get_current_track(mcache);
    auto prev_it = get_prev_track(mode, cur_track_it, begin(pl), end(pl));
    access::set_current_track(mcache, prev_it);
    if (prev_it == end(pl)) {
        return {};
    } else {
        return *prev_it;
    }
}
} // namespace dp

