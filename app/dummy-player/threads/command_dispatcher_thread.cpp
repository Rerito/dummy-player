
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

static std::optional<std::reference_wrapper<dp::command_dispatcher::command_type> > get_command_obj(dp::command_dispatcher& dispatcher, std::string const& cmd_id) {
    std::optional<std::reference_wrapper<dp::command_dispatcher::command_type> > actor;
    try {
        // First, we fetch the appropriate command:
        auto& cmd_bundle = dispatcher.get(cmd_id);
        actor = std::ref(cmd_bundle.first);
    } catch (std::exception& e) {
    }
    return actor;
}

static void run_command(dp::command_dispatcher& dispatcher, shared_message_queue& msg_queue, cmd::raw_command const& cmd) {
    std::string cmd_outcome;
    cmd::MessagePriority msg_type = cmd::MessagePriority::INFO;
    // First, we fetch the appropriate command:
    auto actor = get_command_obj(dispatcher, cmd.cmd_);
    if (!actor) {
        cmd_outcome = "Unknown command `" + cmd.cmd_ + "`, try `help` to see available commands."; 
        msg_type = cmd::MessagePriority::ERROR;
    } else {
        // All good, we can try to run the command
        try {
            cmd_outcome = (*actor)(cmd.args_);
        } catch (std::exception& e) {
            cmd_outcome = "An errored occurred performing command `" + cmd.cmd_ + "`: " + e.what();
            msg_type = cmd::MessagePriority::ERROR;
        }
    }
    // Once the command is performed, we can push the result to the message
    // queue.
    if (!cmd_outcome.empty()) {
        cmd::ui_message msg;
        msg.msg_ = std::move(cmd_outcome);
        msg.prio_ = msg_type;
        push_shared_queue(msg_queue, std::move(msg));
    }
}

void command_dispatcher_main(shared_command_queue& cmd_queue, shared_message_queue& msg_queue, shared_music_store& music_store, player_shared_state& player_state) {
    dp::command_dispatcher dispatcher;
    make_command_dispatcher(music_store, player_state, dispatcher);
    while (true) {
        auto cmd = get_command(cmd_queue);
        run_command(dispatcher, msg_queue, cmd); 
        std::this_thread::sleep_for(10ms);
    }
}
