
#pragma once

#include <optional>
#include "repeat_mode.hpp"

namespace dp {

template <typename MusicCache>
std::optional<typename MusicCache::playlist_type::value_type > next_track(RepeatMode, MusicCache&);

template <typename MusicCache>
std::optional<typename MusicCache::playlist_type::value_type> previous_track(RepeatMode, MusicCache&);

} // namespace dp
