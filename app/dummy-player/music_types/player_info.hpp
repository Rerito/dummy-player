
#pragma once

#include <variant>
#include <vector>
#include <queue>

#include "model/repeat_mode.hpp"

#include "player_status.hpp"
#include "track_metadata.hpp"

struct player_info {
    player_status status_;
    dp::RepeatMode repeat_mode_;
    track_metadata track_;
};

using player_info_update = std::variant<player_status, dp::RepeatMode, track_metadata>;

using player_info_update_queue = std::queue<player_info_update, std::vector<player_info_update> >;
