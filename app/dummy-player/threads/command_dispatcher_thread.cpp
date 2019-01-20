
#include <string>
#include <thread>

#include "command_dispatcher_thread.hpp"
#include "make_command_dispatcher.hpp"

using namespace std::chrono_literals;

static cmd::raw_command get_command(shared_command_queue& cmd_queue) {
    while (true) {
        {
            auto [qrw, lock] = cmd_queue.get_payload();
            auto& q = qrw.get();
            if (!q.empty()) {            
                auto cmd = q.top();
                q.pop();
                return cmd;
            }
        }
        std::this_thread::sleep_for(10ms);
    }
}

static void run_command(dp::command_dispatcher& dispatcher, shared_message_queue& msg_queue, cmd::raw_command const& cmd) {
    std::string cmd_outcome;
    cmd::MessagePriority msg_type = cmd::MessagePriority::INFO;
    try {
        // First, we fetch the appropriate command:
        cmd_outcome = dispatcher.run(cmd.cmd_, cmd.args_);
    } catch (std::exception& e) {
        cmd_outcome = e.what();
        msg_type = cmd::MessagePriority::ERROR;
    }
    if (!cmd_outcome.empty()) {
        auto [mq_rw, lock] = msg_queue.get_payload();
        cmd::ui_message msg;
        msg.msg_ = std::move(cmd_outcome);
        msg.prio_ = msg_type;
        mq_rw.get().emplace(std::move(msg));
    }
}

void command_dispatcher_main(shared_command_queue& cmd_queue, shared_message_queue& msg_queue, shared_music_store& music_store, player_shared_state& player_state) {
    auto dispatcher = make_command_dispatcher(music_store, player_state);
    while (true) {
        auto cmd = get_command(cmd_queue);
        run_command(dispatcher, msg_queue, cmd); 
        std::this_thread::sleep_for(10ms);
    }
}
