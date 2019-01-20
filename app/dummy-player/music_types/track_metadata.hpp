
#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

struct track_metadata {
    std::string title_;
    std::string artist_;

    std::chrono::seconds duration_;
    std::string fname_;
    std::unordered_map<std::string, std::string> attr_;
};

// Extract metadata from an actual track file
// This will allow the streamer thread to play music and the management thread
// to display information...
track_metadata read_track_metadata(std::string const& path);

// A to_string function that will be leveraged through ADL when running
// show_track or show_current_track commands.
std::string to_string(track_metadata const&);

