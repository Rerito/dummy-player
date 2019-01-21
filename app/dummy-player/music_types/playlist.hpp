
#pragma once

#include "model/music_cache.hpp"
#include "model/playlist_shuffle.hpp"
#include "track_metadata.hpp"

using music_store = dp::music_cache<size_t, track_metadata, dp::playlist_shuffler<> >;
