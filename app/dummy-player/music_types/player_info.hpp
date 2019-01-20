
#pragma once

#include "model/repeat_mode.hpp"

#include "player_status.hpp"
#include "track_metadata.hpp"

struct player_info {
    player_status status_;
    dp::RepeatMode repeat_mode_;
    track_metadata track_;
};
