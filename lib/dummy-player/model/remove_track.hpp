
#pragma once

#include <algorithm>

#include "access.hpp"
#include "track_navigation.hpp"

namespace dp {

template <typename MusicCache, typename TrackKey>
void remove_track(MusicCache& mcache, TrackKey const& track_id, RepeatMode mode) {
    bool do_next = false;
    auto& pl = access::get_playlist(mcache);
    auto rmode = (RepeatMode::REPEAT_1 == mode) ? RepeatMode::REPEAT_ALL : mode;
    auto cur_track = access::get_current_track(mcache);
    // Just to throw in case the target track doesn't exist.
    // Better now with an O(1) amortized check than a linear search over the
    // playlist...
    access::get_base_cache(mcache).get(track_id);
    auto tgt_it = std::find_if(
        begin(pl),
        end(pl),
        [&](auto const& rw) { return (rw.get().first) == track_id; }
    );
    auto tgt_idx = tgt_it - begin(pl);
    if (tgt_idx < cur_track) {
        --cur_track;
    } else if (tgt_idx == cur_track) {
        do_next = true;
    }

    // Now we can safely remove the given track...
    // First by removing it from the playlist layer.
    pl.erase(tgt_it);
    // Now let's update the current track index
    access::set_current_track(mcache, cur_track);
    if (do_next && cur_track == size(pl)) {
        next_track(rmode, mcache);
    }
    
    // Then by removing it from the raw storage cache:
    access::get_base_cache(mcache).erase(track_id);
}

} // namespace dp
