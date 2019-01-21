
#pragma once

#include <algorithm>

#include "access.hpp"
#include "track_navigation.hpp"

namespace dp {

template <typename MusicCache, typename TrackKey>
std::optional<typename MusicCache::playlist_type::value_type> remove_track(MusicCache& mcache, TrackKey const& track_id, RepeatMode mode) {
    using track_type = decltype(next_track(mode, mcache));
    track_type track;
    auto& pl = dp::access::get_playlist(mcache);
    // First step is to check if the target track is the current track:
    auto& tgt_track = dp::access::get_base_cache(mcache).get(track_id);
    auto cur_track = dp::access::get_current_track(mcache);
    if (end(pl) != cur_track && &(cur_track->get()) == &tgt_track) {
        // The track to remove is the current track...
        track = next_track((mode == RepeatMode::REPEAT_1) ? RepeatMode::REPEAT_ALL : mode, mcache);
    } else if (end(pl) != cur_track) {
        track = *cur_track;
    }
    // Now we can safely remove the given track...
    // First by removing it from the playlist layer.
    pl.erase(
        std::find_if(
            begin(pl),
            end(pl),
            [&](auto const& rw) { return &(rw.get()) == &tgt_track; }
        )
    );
    //... And from the index
    dp::access::get_playlist_index(mcache).erase(&tgt_track);

    // Then by removing it from the raw storage cache:
    dp::access::get_base_cache(mcache).erase(track_id);
    return track;
}

} // namespace dp
