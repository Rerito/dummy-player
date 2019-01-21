
#pragma once

#include "commands/command_dispatcher.hpp"
#include "commands/command.hpp"
#include "threads/states.hpp"

// Given appropriate  `shared_state`s,
// Construct the command dispatcher that will allow to perform
// playlist management operations:
//   - add_track <track_filename>
//   - rm_track <track_id>
//   - set_track <track_id>
//   - next_track
//   - previous_track
//   - show_current_track
//   - show_track <track_id>
//   - pause
//   - play
//   - repeat <repeat_mode>
//   - shuffle
void make_command_dispatcher(shared_music_store& music_store, player_shared_state& player_state, dp::command_dispatcher& dispatcher);
