
#include <thread>
#include <sstream>
#include "threads/threads.hpp"

using namespace std::chrono_literals;

int main() {

    // All the shared states
    shared_message_queue msg_queue;
    shared_command_queue cmd_queue;
    ui_shared_state ui_state;
    player_shared_state player_state;
    shared_music_store mstore;

    std::thread ui_thread([&]() {
        user_input_main(cmd_queue, ui_state);
    });

    std::thread status_thread([&]() {
        ui_status_main(msg_queue, ui_state);
    });

    ui_thread.join();
    status_thread.join();

    return 0;
}
