
#pragma once

#include <functional>

#include "access.hpp"
#include "utils/forward_macro.hpp"

namespace dp {

template <typename MusicCache, typename TrackKey, typename... TrackArgs>
void add_track(MusicCache& mcache, TrackKey const& track_id, TrackArgs&&... args) {
    auto& new_track = access::get_base_cache(mcache).emplace(track_id, CPPFWD(args)...);
    access::get_playlist(mcache).emplace_back(std::ref(access::get_last_base_item(mcache)));
    access::refresh_current_track(mcache);
}

} // namespace dp

