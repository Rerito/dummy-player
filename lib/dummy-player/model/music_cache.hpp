
#pragma once

#include <vector>
#include "datastruct/cache.hpp"
#include "utils/make_ref_wrapper_vector.hpp"

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

    friend class access;

public:
    using base_type = cache<TrackKey, Track, Hash, Eq>;
    using playlist_type = std::vector<std::reference_wrapper<Track> >;
    using playlist_iterator = typename playlist_type::iterator;
    using track_type = Track;
private:
    playlist_type playlist_;
    std::unordered_map<Track*, TrackKey> playlist_idx_;

    playlist_iterator current_track_it_;
    std::optional<std::reference_wrapper<Track> > current_track_;

    // When a playlist reordering occurs, we must update the playlist iterator
    // so that it points to the correct track...
    // `current_track_` is never invalidated unless it is removed from the
    // playlist.
    void refresh_current_track() {
        if (current_track_) {
            current_track_it_ = std::find_if(begin(playlist_), end(playlist_), [&](auto const& tr) { return &(current_track_->get()) == &(tr.get()); });
        } else {
            current_track_it_ = end(playlist_);
        }
    }

public:
    music_cache() : playlist_(), current_track_it_(end(playlist_)), current_track_() {}
    // We will try to have a very minimalistic approach with regard to the
    // features we embed into the music_cache...
    // This will allow for better encapsulation and modularity
    void reorder(random_shuffle_tag) {
        using std::begin;
        using std::end;
        ShufflePolicy::shuffle(begin(playlist_), end(playlist_));
    }

    void reorder(native_order_tag) {
        playlist_ = make_ref_wrapper_vector(base_type::elems_); 
    }

    std::optional<std::reference_wrapper<Track const> > get_current_track() const {
        if (current_track_) {
            return std::cref(current_track_->get());
        } else {
            return {};
        }
    }
};

} // namespace dp
