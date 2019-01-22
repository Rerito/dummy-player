
#pragma once

#include "states.hpp"

void streamer_main(shared_message_queue& msg_queue,
                   shared_command_queue& cmd_queue,
                   player_shared_state& player_state,
                   shared_playback_status& playback_status);

