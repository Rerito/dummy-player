
#pragma once

#include <string>

#include "access.hpp"

namespace dp {

namespace detail {
template <typename TrackKey, typename Track, typename OStream>
void dump_track(TrackKey const& id, Track const& track, OStream& ostr) {
    using std::to_string;
    ostr << "Track (" << to_string(id) << "):\n"
         << "|___ " << to_string(track) << "\n";
}
} // namespace detail


template <typename MusicCache, typename TrackKey, typename OStream>
void info_track(MusicCache& mcache, TrackKey const& track_id, OStream& ostr) {
    auto const& track = access::get_base_cache(mcache).get(track_id);
    detail::dump_track(track_id, track, ostr);
}

template <typename MusicCache, typename OStream>
void info_playlist(MusicCache& mcache, OStream& ostr) {
    auto const& pl = access::get_playlist(mcache);
    ostr << "Playlist information:\n";
    for (auto const& trk : pl) {
        detail::dump_track(trk.get().first, trk.get().second, ostr);
    }
}

} // namespace dp
