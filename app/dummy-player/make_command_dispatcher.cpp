
#include <sstream>
#include <string>

#include "threads/states.hpp"
#include "commands/command_dispatcher.hpp"
#include "commands/command.hpp"
#include "make_command_dispatcher.hpp"
#include "model/add_track.hpp"
#include "model/remove_track.hpp"
#include "model/track_navigation.hpp"
#include "model/set_track.hpp"
#include "model/unique.hpp"

static std::string help_function(dp::command_dispatcher const& dispatcher) {
    std::stringstream sstr;
    sstr << "Command usage:\n";
    dispatcher.help(sstr);   
    return sstr.str();
}

void make_command_dispatcher(shared_music_store& music_store, player_shared_state& player_state, dp::command_dispatcher& dispatcher) {
    auto add_track_fn = dp::command<std::string(size_t, std::string const&)>([&](size_t track_id, std::string const& track_file) {
        auto [store_rw, lock] = music_store.get_payload();
        dp::add_track(store_rw.get(), track_id, read_track_metadata(track_file));
        return std::string("add_track ran successfully");
    });

    auto next_track_fn = dp::command<std::string()>([&]() {
        auto [store_rw, lock] = music_store.get_payload();
        {
            auto [playerst_rw, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
            dp::next_track(playerst_rw.get().repeat_mode_, store_rw.get());
        }
        {
            auto [pst, pst_wlock] = player_state.get_payload();
            update_player_track(store_rw.get(), pst.get(), true);
        }
        return std::string("next_track ran successfully");
    });

    auto prev_track_fn = dp::command<std::string()>([&]() {
        auto [store_rw, lock] = music_store.get_payload();
        {
            auto [playerst_rw, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
            dp::previous_track(playerst_rw.get().repeat_mode_, store_rw.get());
        }
        {
            auto [pst, pst_wlock] = player_state.get_payload();
            update_player_track(store_rw.get(), pst.get(), true);
        }
        return std::string("prev_track ran successfully");
    });

    auto rm_track_fn = dp::command<std::string(size_t)>([&](size_t track_id) {
        auto [store_rw, wlock] = music_store.get_payload();
        {
            auto [playerst_rw, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
            dp::remove_track(store_rw.get(), track_id, playerst_rw.get().repeat_mode_);
        }
        {
            auto [pst, pst_wlock] = player_state.get_payload();
            update_player_track(store_rw.get(), pst.get());
        }
        return "rm_track ran successfully";
    });

    auto repeat_mode_fn = dp::command<std::string(dp::RepeatMode)>([&](dp::RepeatMode mode) {
        auto [playerst_rw, wlock] = player_state.get_payload();
        playerst_rw.get().repeat_mode_ = mode;
        return "repeat_mode ran successfully";
    });

    auto set_track_fn = dp::command<std::string(size_t)>([&](size_t track_id) {
        auto [store_rw, wlock] = music_store.get_payload();
        dp::set_track(store_rw.get(), track_id);
        {
            auto [pst, pst_wlock] = player_state.get_payload();
            update_player_track(store_rw.get(), pst.get(), true);
        }
        return "set_track ran successfully";
    });

    auto play_fn = dp::command<void()>([&]() {
        auto [plr, wlock] = player_state.get_payload();
        plr.get().status_ = player_status::PLAYING;
    });

    auto pause_fn = dp::command<void()>([&]() {
        auto [plr, wlock] = player_state.get_payload();
        plr.get().status_ = player_status::PLAYING;
    });

    auto unique_fn = dp::command<void()>([&]() {
        auto rmode = dp::RepeatMode::NO_REPEAT;
        {
            auto [plr, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
            rmode = plr.get().repeat_mode_;
        }
        {
            auto [mcache, wlock] = music_store.get_payload();
            dp::unique(mcache.get(), rmode, track_metadata_less {}, track_metadata_equal_to {});
            {
                auto [plr, plr_wlock] = player_state.get_payload();
                update_player_track(mcache.get(), plr.get());
            }
        }
    });

    auto info_playlist_fn = dp::command<std::string()>([&]() -> std::string { return {}; });

    auto quit_fn = dp::command<void()>([]() { std::exit(0); });

    dispatcher.register_command("add_track", std::move(add_track_fn), "add_track <track_id> <track_file>: add the given file to the playlist");
    dispatcher.register_command("next_track", std::move(next_track_fn), "next_track: Skip to the next track. Stops playback if repeat is disabled and end of playlist is reached.");
    dispatcher.register_command("prev_track", std::move(prev_track_fn), "prev_track: Go to the previous track.");
    dispatcher.register_command("set_track", std::move(set_track_fn), "set_track <track_id>: set the current track to the given track.");
    dispatcher.register_command("rm_track", std::move(rm_track_fn), "rm_track <track_id>: remove the given track file from the playlist");
    dispatcher.register_command("unique", std::move(unique_fn), "unique: remove all duplicate tracks from the playlist");
    dispatcher.register_command("repeat_mode", std::move(repeat_mode_fn), "repeat_mode <NONE|ONE|ALL>: set the repeat mode to the given setting");
    dispatcher.register_command("play", std::move(play_fn), "play: start playing (if there is a track to read)");
    dispatcher.register_command("pause", std::move(pause_fn), "pause: pauses the playback thread");
    dispatcher.register_command("quit", std::move(quit_fn), "quit: exit the program... Brutally! ;o");
    dispatcher.register_command("help", dp::command<std::string()>([&]() { return help_function(dispatcher); }), "help: print command descriptions");
}



