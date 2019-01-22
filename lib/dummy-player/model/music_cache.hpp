
#pragma once

#include <vector>
#include "datastruct/cache.hpp"

#include <optional>
#include <algorithm>
#include <functional>

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
    using item = typename base_type::list_t::value_type;
    using playlist_item = std::reference_wrapper<item>; 
    using const_playlist_item = std::reference_wrapper<item const>;
    using playlist_type = std::vector<playlist_item>;
    using playlist_iterator = typename playlist_type::iterator;
private:
    playlist_type playlist_;

    size_t current_track_idx_;
    // This is still needed not to lose the current track when a shuffle occurs
    // It is cheap though since playlist_item is a reference_wrapper
    std::optional<playlist_item> current_track_;

    // When a playlist reordering occurs, we must update the playlist iterator
    // so that it points to the correct track...
    // `current_track_` is never invalidated unless it is removed from the
    // playlist.
    void refresh_current_track() {
        if (current_track_) {
            current_track_idx_ = std::find_if(begin(playlist_), end(playlist_), [&](auto const& tr) { return (current_track_->get().first) == (tr.get().first); }) - begin(playlist_);
        } else {
            current_track_idx_ = size(playlist_);
        }
    }

public:
    music_cache() : playlist_(), current_track_idx_(0u), current_track_() {}
    // We will try to have a very minimalistic approach with regard to the
    // features we embed into the music_cache...
    // This will allow for better encapsulation and modularity
    void reorder(random_shuffle_tag) {
        using std::begin;
        using std::end;
        ShufflePolicy::shuffle(begin(playlist_), end(playlist_));
        refresh_current_track();
    }

    void reorder(native_order_tag) {
        playlist_.clear(); 
        std::transform(
            begin(base_type::elems_),
            end(base_type::elems_),
            std::back_inserter(playlist_),
            [](auto& list_it) {
                return std::ref(list_it);
            }
        );
        refresh_current_track();
    }

    std::optional<const_playlist_item> get_current_track() const {
        if (current_track_) {
            return std::cref(current_track_->get());
        } else {
            return {};
        }
    }
};

} // namespace dp
