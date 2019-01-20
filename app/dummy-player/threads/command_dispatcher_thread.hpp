
#pragma once

#include "threads/states.hpp"

void command_dispatcher_main(shared_command_queue& cmd_queue, shared_message_queue& msg_queue, shared_music_store& music_store, player_shared_state& player_state);

