
#include <chrono>
#include <thread>

#include "streamer_thread.hpp"

using namespace std::chrono_literals;

static void playback_main(shared_state<std::pair<bool, bool> >& run,
                          shared_message_queue& msg_queue,
                          shared_command_queue& cmd_queue,
                          player_shared_state& player_state,
                          shared_playback_status& pbck_status_sh,
                          track_bundle_t track_bundle) {
    std::chrono::high_resolution_clock clk;
    bool fetch_next_track = false;
    // Tell the main streamer thread I am alive
    auto& track = track_bundle.second;
    push_shared_queue(msg_queue, cmd::ui_message { "Beginning playing track " + track.title_ + " by " + track.artist_, cmd::MessagePriority::INFO});
    apply_to_shared_state(run, [](auto& r) { r.second = true; });
    size_t playing_cycles = 0u;
    playback_status pbck_status { 0ms, std::chrono::duration_cast<std::chrono::milliseconds>(track.duration_)};
    while (get_shared_state(run).first) {
        auto playing_mode = apply_to_shared_state(
            const_cast<player_shared_state const&>(player_state),
            [](auto const& st) { return st.status_; }
        );
        auto tp = clk.now();
        std::this_thread::sleep_for(100ms);
        auto etp = clk.now();
        if (playing_mode == player_status::PLAYING) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(etp - tp);
            pbck_status.elapsed_ += elapsed;
            playing_cycles++;
        }
        if (pbck_status.elapsed_ > pbck_status.track_duration_) {
            fetch_next_track = true;
            break;
        }
        if (100 == playing_cycles) {
            playing_cycles = 0;
            apply_to_shared_state(pbck_status_sh, [&](auto& st) { st.elapsed_ = pbck_status.elapsed_; });
        }

    }
    if (fetch_next_track) {
        push_shared_queue(cmd_queue, cmd::raw_command { "next_track", {}, cmd::CommandPriority::CRITICAL});
    }
    // Signal the main streamer thread I am terminating
    apply_to_shared_state(run, [](auto& r) { r.second = false; });
}

void streamer_main(shared_message_queue& msg_queue,
                   shared_command_queue& cmd_queue,
                   player_shared_state& player_state,
                   shared_playback_status& playback_status) {
    shared_state<std::pair<bool, bool> > child_thread_status(true, false);
    while (true) {
        // Did we receive a new track to play?
        bool new_track_to_play = apply_to_shared_state(
            const_cast<player_shared_state const&>(player_state),
            [](auto const& st) { return st.update_track_; }
        );
        if (new_track_to_play) {
            auto [strw, wlock] = player_state.get_payload();
            auto& st = strw.get();
            // We must signal the hypothetical playback thread to terminate
            if (st.update_track_) { // Double lock pattern
                apply_to_shared_state(child_thread_status, [](auto& cst) { cst.first = false; });
                while (get_shared_state(child_thread_status).second) {
                    std::this_thread::sleep_for(10ms);
                }
                apply_to_shared_state(child_thread_status, [](auto& cst) { cst.first = true; });
                // Launch the playback thread
                // If and only if the optional track is valid:
                if (st.track_) {
                    std::thread t([&]() {
                        playback_main(
                            child_thread_status,
                            msg_queue,
                            cmd_queue,
                            player_state,
                            playback_status,
                            (*st.track_)
                        );
                    });
                    t.detach();
                }
                st.update_track_ = false;
            }
        }
        std::this_thread::sleep_for(10ms);
    }
}

static std::optional<std::pair<size_t, track_metadata> > get_current_track_from_store(music_store const& ms) {
    auto cur_track = ms.get_current_track();
    if (cur_track) {
        return std::make_pair(cur_track->get().first, cur_track->get().second); 
    } else {
        return {};
    }
}

static bool track_eq(std::optional<track_bundle_t> const& t1,
                     std::optional<track_bundle_t> const& t2) {
    if (t1) {
        return t2 && t1->first == t2->first;
    } else {
        return !t2;
    }
}

void update_player_track(music_store const& ms, player_info& plr_state, bool bypass_comp) {
    auto cur_track = get_current_track_from_store(ms);
    if (bypass_comp || !track_eq(cur_track, plr_state.track_)) {
        plr_state.track_ = std::move(cur_track);
        plr_state.update_track_ = true;
    }
}
