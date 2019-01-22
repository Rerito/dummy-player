
#pragma once

#include <algorithm>

#include "access.hpp"

namespace dp {

template <typename MusicCache, typename TrackKey>
void set_track(MusicCache& mcache, TrackKey const& track_id) {
    auto& pl = access::get_playlist(mcache);
    // Let's first access the base cache item to prevent a linear search if the
    // track does not even exist...
    access::get_base_cache(mcache).get(track_id);
    auto new_track_it = std::find_if(begin(pl), end(pl), [&](auto& v) { return track_id == (v.get().first); });
    if (end(pl) == new_track_it) {
        throw std::runtime_error("Found the desired track in storage but it was not in the playlist!");
    }
    access::set_current_track(mcache, new_track_it - begin(pl));
}

template <typename MusicCache>
void set_track(MusicCache& mcache) {
    auto& pl = access::get_playlist(mcache);
    access::set_current_track(mcache, size(pl));
}

} // namespace dp
