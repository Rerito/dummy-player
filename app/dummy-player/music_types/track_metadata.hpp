
#pragma once

#include <chrono>
#include <string>
#include <unordered_map>

struct track_metadata {
    std::string title_;
    std::string artist_;

    std::chrono::seconds duration_;
    // Ideally I would have used (std|boost)::filesystem::path here... 
    std::string fname_;
    std::unordered_map<std::string, std::string> attr_;
};

struct track_metadata_equal_to {
    bool operator()(track_metadata const& t1, track_metadata const& t2) const {
        return t1.title_ == t2.title_ && t1.artist_ == t2.artist_;
    }
};

struct track_metadata_less {
    bool operator()(track_metadata const& t1, track_metadata const& t2) const {
        return t1.artist_ < t2.artist_ || (t1.artist_ == t2.artist_ && t1.title_ < t2.title_);
    }
};

// Extract metadata from an actual track file
// This will allow the streamer thread to play music and the management thread
// to display information...
track_metadata read_track_metadata(std::string const& path);

// A to_string function that will be leveraged through ADL when running
// show_track or show_current_track commands.
std::string to_string(track_metadata const&);

