
#include <chrono>
#include <thread>

#include "streamer_thread.hpp"

using namespace std::chrono_literals;

static void playback_main(shared_state<bool>& run,
                          player_shared_state& player_state,
                          shared_playback_status& pbck_status_sh) {
    std::chrono::high_resolution_clock clk;
    auto track = apply_to_shared_state(
        const_cast<player_shared_state const&>(player_state),
        [](auto const& pstate) { return pstate.track_; }
    );
    size_t playing_cycles = 0u;
    playback_status pbck_status { 0ms, std::chrono::duration_cast<std::chrono::milliseconds>(track.duration_)};
    while (get_shared_state(run)) {
        auto playing_mode = apply_to_shared_state(
            const_cast<player_shared_state const&>(player_state),
            [](auto const& st) { return st.status_; }
        );
        auto tp = clk.now();
        std::this_thread::sleep_for(10ms);
        auto etp = clk.now();
        if (playing_mode == player_status::PLAYING) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(tp - etp);
            pbck_status.elapsed_ += elapsed;
            playing_cycles++;
        }
        if (pbck_status.elapsed_ > pbck_status.track_duration_) {
            break;
        }
        if (100 == playing_cycles) {
            playing_cycles = 0;
            apply_to_shared_state(pbck_status_sh, [&](auto& st) { st.elapsed_ = pbck_status.elapsed_; });
        }

    }
    apply_to_shared_state(run, [](bool& r) { r = false; });
}

void streamer_main(shared_message_queue& msg_queue,
                   shared_command_queue& cmd_queue,
                   player_shared_state& player_state,
                   shared_playback_status& playback_status) {
    while (true) {
                
    }
}
