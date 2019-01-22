
#pragma once

#include "utils/forward_macro.hpp"
#include <optional>

#include "track_navigation_fwd.hpp"

namespace dp {

// This class allow the different features to use the music_cache
class access {

    // Friend services...

    // Player management services:
    template <typename MusicCache>
    friend std::optional<typename MusicCache::playlist_type::value_type> next_track(RepeatMode, MusicCache&);

    template <typename MusicCache>
    friend std::optional<typename MusicCache::playlist_type::value_type> previous_track(RepeatMode, MusicCache&);

    template <typename MusicCache, typename TrackKey>
    friend void set_track(MusicCache&, TrackKey const&);

    template <typename MusicCache>
    friend void set_track(MusicCache&);

    // Playlist management services:
    template <typename MusicCache>
    friend void shuffle_playlist(MusicCache&);

    template <typename MusicCache, typename TrackKey, typename... TrackArgs>
    friend void add_track(MusicCache& music_cache, TrackKey const&, TrackArgs&&...);

    template <typename MusicCache, typename TrackKey>
    friend std::optional<typename MusicCache::playlist_type::value_type> remove_track(MusicCache& music_cache, TrackKey const&, RepeatMode);

    template <typename MusicCache, typename TrackLess, typename TrackEqual>
    friend void unique(MusicCache&, RepeatMode, TrackLess&&, TrackEqual&&);

    // Informational services:
    template <typename MusicCache, typename OStream>
    friend void info_playlist(MusicCache&, OStream&);

    template <typename MusicCache, typename TrackKey>
    friend std::string info_track(MusicCache&, TrackKey&);

    template <typename MusicCache>
    static decltype(auto) get_playlist(MusicCache&& music_cache) {
        return (CPPFWD(music_cache).playlist_);
    }

    template <typename MusicCache>
    static auto const& get_playlist(MusicCache const& music_cache) {
        return music_cache.playlist_;
    }

    template <typename MusicCache>
    static auto& get_playlist_index(MusicCache& mcache) {
        return mcache.playlist_idx_;
    }

    template <typename MusicCache>
    static typename MusicCache::base_type& get_base_cache(MusicCache& music_cache) {
        return music_cache;
    }

    template <typename MusicCache>
    static typename MusicCache::base_type::list_t::reference get_last_base_item(MusicCache& music_cache) {
        return music_cache.elems_.back();
    }

    template <typename MusicCache>
    static void refresh_current_track(MusicCache& mcache) {
        mcache.refresh_current_track();
    }

    template <typename MusicCache>
    static auto get_current_track(MusicCache const& mcache) {
        return mcache.current_track_it_;
    }
    
    template <typename MusicCache, typename It>
    static auto set_current_track(MusicCache& mcache, It new_track_it) {
        mcache.current_track_it_ = new_track_it;
        if (end(mcache.playlist_) == new_track_it) {
            mcache.current_track_ = std::nullopt;
        } else {
            mcache.current_track_ = *new_track_it;
        }
    }
};

} // namespace dp
