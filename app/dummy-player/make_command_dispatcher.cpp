
#include <sstream>
#include <string>

#include "threads/states.hpp"
#include "commands/command_dispatcher.hpp"
#include "commands/command.hpp"
#include "make_command_dispatcher.hpp"
#include "model/add_track.hpp"
#include "model/remove_track.hpp"
#include "model/track_navigation.hpp"

static std::string help_function(dp::command_dispatcher const& dispatcher) {
    std::stringstream sstr;
    sstr << "Command usage:\n";
    dispatcher.help(sstr);   
    return sstr.str();
}

void make_command_dispatcher(shared_music_store& music_store, player_shared_state& player_state, dp::command_dispatcher& dispatcher) {
    auto add_track_fn = dp::command<std::string(std::string const&)>([&](std::string const& track_id) {
        auto [store_rw, lock] = music_store.get_payload();
        dp::add_track(store_rw.get(), track_id, read_track_metadata(track_id));
        return std::string("add_track ran successfully");
    });

    auto next_track_fn = dp::command<std::string()>([&]() {
        auto [store_rw, lock] = music_store.get_payload();
        auto [playerst_rw, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
        dp::next_track(playerst_rw.get().repeat_mode_, store_rw.get());
        return std::string("next_track ran successfully");
    });

    auto prev_track_fn = dp::command<std::string()>([&]() {
        auto [store_rw, lock] = music_store.get_payload();
        auto [playerst_rw, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
        dp::previous_track(playerst_rw.get().repeat_mode_, store_rw.get());
        return std::string("prev_track ran successfully");
    });

    auto rm_track_fn = dp::command<std::string(std::string const&)>([&](std::string const& track_id) {
        auto [store_rw, wlock] = music_store.get_payload();
        auto [playerst_rw, rlock] = const_cast<player_shared_state const&>(player_state).get_payload();
        dp::remove_track(store_rw.get(), track_id, playerst_rw.get().repeat_mode_);
        return "rm_track ran successfully";
    });
    auto quit_fn = dp::command<void()>([]() { std::terminate(); });

    dispatcher.register_command("add_track", std::move(add_track_fn), "add_track <track_file>: add the given file to the playlist");
    dispatcher.register_command("next_track", std::move(next_track_fn), "next_track: Skip to the next track. Stops playback if repeat is disabled and end of playlist is reached.");
    dispatcher.register_command("prev_track", std::move(prev_track_fn), "prev_track: Go to the previous track.");
    dispatcher.register_command("rm_track", std::move(rm_track_fn), "rm_track <track_file>: remove the given track file from the playlist");
    dispatcher.register_command("quit", std::move(quit_fn), "quit: exit the program... Brutally! ;o");
    dispatcher.register_command("help", dp::command<std::string()>([&]() { return help_function(dispatcher); }), "help: print command descriptions");
}



