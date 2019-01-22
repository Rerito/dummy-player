
#pragma once

#include <variant>
#include <vector>
#include <queue>

#include "model/repeat_mode.hpp"

#include "player_status.hpp"
#include "track_metadata.hpp"

using track_bundle_t = std::pair<size_t, track_metadata>;

struct player_info {
    player_status status_;
    dp::RepeatMode repeat_mode_;
    std::optional<track_bundle_t> track_;
    bool update_track_;
};

struct playback_status {
    std::chrono::milliseconds elapsed_;
    std::chrono::milliseconds track_duration_;
};

using player_info_update = std::variant<player_status, dp::RepeatMode, track_metadata>;

using player_info_update_queue = std::queue<player_info_update, std::vector<player_info_update> >;

