
#pragma once

#include <string>

#include "thread/shared_state.hpp"
#include "music_types/playlist.hpp"
#include "music_types/player_info.hpp"
#include "commands/types.hpp"

using ui_shared_state = shared_state<std::string>;
using shared_music_store = shared_state<music_store>;
using player_shared_state = shared_state<player_info>;

using shared_command_queue = shared_state<cmd::command_queue>;
using shared_message_queue = shared_state<cmd::message_queue>;




