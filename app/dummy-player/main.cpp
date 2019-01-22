
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

    // The streamer thread is responsible for launching playback streams
    // And listens to any change on the current track.
    // During its life, the playback thread will push messages and "next_track"
    // commands.
    std::thread streamr_thread([&]() {
        streamer_main(
            msg_queue,
            cmd_queue,
            player_state,
            playback_status
        );
    });

    // The ui thread only pushes user input commands to the command queue
    std::thread ui_thread([&]() {
        user_input_main(cmd_queue);
    });

    // The command threads launches the commands sent either by:
    //   - The user through the user input thread
    //   - The playback thread once it has finished playing a track
    // A command can generate output to be printed, be it a result of some kind
    // or an error. Either way, a message is published in the message queue.
    std::thread cmd_thread([&]() {
        command_dispatcher_main(cmd_queue, msg_queue, mstore, player_state);
    });

    // The status thread only prints messages
    std::thread status_thread([&]() {
        ui_status_main(msg_queue);
    });

    ui_thread.join();
    cmd_thread.join();
    status_thread.join();
    streamr_thread.join();

    return 0;
}
