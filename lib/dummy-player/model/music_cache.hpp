
#pragma once

#include <vector>
#include "datastruct/cache.hpp"
#include "utils/make_iterator_vector.hpp"

#include <optional>

namespace dp {

struct random_shuffle_tag {};
struct native_order_tag {};

// The music_cache will handle a collection of tracks
template <typename TrackKey,
          typename Track,
          typename ShufflePolicy,
          typename Hash = std::hash<TrackKey>,
          typename Eq = std::equal_to<TrackKey> >
class music_cache : private cache<TrackKey, Track, Hash, Eq>, private ShufflePolicy {
    using base_type = cache<TrackKey, Track, Hash, Eq>;
    using playlist_type = std::vector<typename base_type::list_iterator>;
    using playlist_iterator = typename playlist_type::iterator;
    playlist_type playlist_;

    playlist_iterator current_track_it_;
    typename base_type::list_iterator current_track_;

    // When a playlist reordering occurs, we must update the playlist iterator
    // so that it points to the correct track...
    // `current_track_` is never invalidated unless it is removed from the
    // playlist.
    void refresh_current_track() {
        current_track_it_ = find(begin(playlist_), end(playlist_), current_track_);
    }

public:
    
    std::optional<std::reference_wrapper<Track> > next() {
        return {};
    }

    void reorder(random_shuffle_tag) {
        ShufflePolicy::shuffle(playlist_);
    }

    void reorder(native_order_tag) {
        playlist_ = make_iterator_vector(base_type::elems_); 
    }
};

} // namespace dp
