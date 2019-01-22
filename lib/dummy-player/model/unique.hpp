
#pragma once

#include "access.hpp"
#include "remove_track.hpp"
#include <unordered_set>

namespace dp {

// We could do without the equality operator but this would considerably
// increase the number of comparisons...
template <typename MusicCache, typename TrackLess, typename TrackEqual>
void unique(MusicCache& mcache, RepeatMode mode, TrackLess&& less, TrackEqual&& equals) {
    auto track_comp = [&](auto const& tr1, auto const& tr2) {
        auto& tr1_ = tr1.get();
        auto& tr2_ = tr2.get();
        return less(tr1_.second, tr2_.second) ||
            (equals(tr1_.second, tr2_.second) && tr1_.first < tr2_.first);
    };
    auto rmode = (mode == RepeatMode::REPEAT_1) ? RepeatMode::NO_REPEAT : mode;
    // Begin by establishing the set of tracks to remove
    auto pl_copy = dp::access::get_playlist(mcache);
    // We sort the playlist using the specified track < and == operators
    // So that in the end "equal" tracks are sorted by increasing track id
    // This way, for any duplicates, we will only keep the track with the lowest
    // id.
    std::stable_sort(
        begin(pl_copy),
        end(pl_copy),
        track_comp
    );
    auto sorted_pl = pl_copy;
    pl_copy.erase(
        std::unique(
            begin(pl_copy),
            end(pl_copy),
            [&](auto const& tr1, auto const& tr2) { return equals(tr1.get().second, tr2.get().second); }
        ),
        end(pl_copy)
    );
    
    decltype(pl_copy) to_remove;
    to_remove.reserve(size(sorted_pl) - size(pl_copy));
    std::set_difference(
        begin(sorted_pl), end(sorted_pl),
        begin(pl_copy), end(pl_copy),
        std::back_inserter(to_remove),
        track_comp
    );
    // Then, remove the corresponding tracks
    for (auto const& track: to_remove) {
        remove_track(mcache, track.get().first, rmode);
    }
}

} // namespace dp
