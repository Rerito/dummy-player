
#pragma once

#include "model/music_cache.hpp"
#include "model/playlist_shuffle.hpp"
#include "track_metadata.hpp"

using music_store = dp::music_cache<std::string, track_metadata, dp::playlist_shuffler<> >;
