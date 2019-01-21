
#include "track_metadata.hpp"
#include <fstream>
#include <sstream>
#include <string_view>
#include <stdexcept>

static std::chrono::seconds parse_duration(std::string const& duration) {
    return std::chrono::seconds { std::stoul(duration) };
}

static std::pair<std::string, std::string> parse_kv_line(std::string const& line) {
    using std::data;
    auto pos = line.find_first_of("=");
    if (std::string::npos == pos) {
        throw std::invalid_argument("line");
    }
    return std::pair<std::string, std::string>(std::string_view(data(line), pos), std::string_view(data(line) + pos + 1, size(line) - pos - 1));
}

static std::unordered_map<std::string, std::string> parse_kv_file(std::string const& path) {
    std::unordered_map<std::string, std::string> kv_map;
    std::ifstream ifile(path);
    std::string line;
    while (std::getline(ifile, line)) {
        try {
            kv_map.emplace(parse_kv_line(line));
        } catch (std::invalid_argument&) {
            continue;
        }
    }
    return kv_map;
}

// Pseudo track files will consist of key-value pairs lines:
// key=value
//
// Expected keys are:
//  artist
//  title
//  duration
//
// Any additionnal (key, value) pair will be registered in the attribute map of
// the track_metadata structure.
track_metadata  read_track_metadata(std::string const& path) {
    auto kv_map = parse_kv_file(path);

    track_metadata md { kv_map["artist"], kv_map["title"], parse_duration(kv_map["duration"])  };
    kv_map.erase("artist");
    kv_map.erase("title");
    kv_map.erase("duration");
    md.attr_ = std::move(kv_map);

    return md;
}

std::string to_string(track_metadata const& md) {
    using std::to_string;
    std::stringstream sstr;
    sstr << "artist: " << md.artist_ << "\n"
         << "title: " << md.title_ << "\n"
         << "duration: " << to_string(md.duration_.count()) << "\n";
    for (auto const& at : md.attr_) {
        sstr << at.first << ": " << at.second << "\n";
    }
    return sstr.str();
}
