
#pragma once

#include "command.hpp"

#include <string>
#include <unordered_map>

namespace dp {

class command_dispatcher {
    
public:
    using command_type = std::function<std::string(std::vector<std::string> const&)>;
    using command_bundle_type = std::pair<command_type, std::string>;
    template <typename FTor>
    void register_command(std::string command_name, FTor&& f, std::string usage) {
        commands_.emplace(
            command_name,
            command_bundle_type(command_type(std::forward<FTor>(f)), std::move(usage))
        );
    }

    auto& get(std::string const& cmd) {
        return commands_.at(cmd);
    }
    std::string run(std::string const& cmd, std::vector<std::string> const& args) const {
        return commands_.at(cmd).first(args);
    }

    template <typename OStream>
    void help(OStream& out) const {
        for (auto const& cmd : commands_) {
            out << cmd.second.second << "\n";
        }
    }

private:
    std::unordered_map<std::string, command_bundle_type> commands_;
};

} // namespace dp
