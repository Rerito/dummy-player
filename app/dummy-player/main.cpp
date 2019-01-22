
#include <thread>
#include <sstream>
#include "threads/threads.hpp"
#include "thread/safe_thread.hpp"

using namespace std::chrono_literals;

int main() {

    // All the shared states
    shared_message_queue msg_queue;
    shared_command_queue cmd_queue;
    player_shared_state player_state;
    shared_playback_status playback_status;
    shared_music_store mstore;
    shared_state<std::exception_ptr> status_eptr;

    std::thread streamr_thread([&]() {
        streamer_main(
            msg_queue,
            cmd_queue,
            player_state,
            playback_status
        );
    });

    std::thread ui_thread([&]() {
        user_input_main(cmd_queue);
    });

    std::thread cmd_thread([&]() {
        command_dispatcher_main(cmd_queue, msg_queue, mstore, player_state);
    });
    safe_thread<> status_thread([&]() {
        ui_status_main(msg_queue);
    }, status_eptr);

    ui_thread.join();
    cmd_thread.join();
    status_thread.join();

    return 0;
}
