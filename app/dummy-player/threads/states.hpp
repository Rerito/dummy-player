
#pragma once

#include <string>

#include "utils/forward_macro.hpp"
#include "thread/shared_state.hpp"
#include "music_types/playlist.hpp"
#include "music_types/player_info.hpp"
#include "commands/types.hpp"

using ui_shared_state = shared_state<std::string>;
using shared_music_store = shared_state<music_store>;
using player_shared_state = shared_state<player_info>;
using shared_player_update_queue = shared_state<player_info_update_queue>;

using shared_command_queue = shared_state<cmd::command_queue>;
using shared_message_queue = shared_state<cmd::message_queue>;

template <typename Queue, typename... Args>
inline void push_shared_queue(shared_state<Queue>& msg_queue, Args&&... args) {
    auto [q_refwrap, wlock] = msg_queue.get_payload();
    q_refwrap.get().emplace(CPPFWD(args)...);
}

template <typename Queue>
inline std::optional<typename Queue::value_type> pop_shared_queue(shared_state<Queue>& queue) {
    std::optional<typename Queue::value_type> item;
    auto [q_refwrap, wlock] = queue.get_payload();
    auto& q = q_refwrap.get();
    if (!q.empty()) {
        item = q.top();
        q.pop();
    }
    return item;
}


