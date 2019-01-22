
#include <iostream>
#include <thread>
#include <string>
#include <vector>

#include "user_input_thread.hpp"

static void dump_cmd(cmd::raw_command const& cmd) {
    std::cout << "Command is: " << cmd.cmd_ << "\n";
    for (auto const& arg : cmd.args_) {
        std::cout << arg << " ";
    }
    std::cout << std::endl;
}

static std::vector<std::string> split_string(std::string str, std::string const& delim) {
    std::vector<std::string> acc;
    size_t pos = 0;
    while ((pos = str.find(delim)) != std::string::npos) {
        std::string token = str.substr(0, pos);
        if (!token.empty()) {
            acc.push_back(token);
        }
        str.erase(0, pos + size(delim));
    }
    acc.push_back(str);
    return acc;
}

static std::string get_command() {
    std::string cmd;
    std::cout << "> ";
    std::getline(std::cin, cmd);
    return cmd;
}


static std::pair<std::string, std::vector<std::string> > process_command(std::string const& cmd) {
    // With proper parsing libraries, a little spirit parser would have been way
    // better than this simple split on white spaces.
    auto split_cmd = split_string(cmd, " ");
    if (split_cmd.empty()) {
        return {};
    } else {
        auto first = split_cmd.front();
        split_cmd.erase(begin(split_cmd));
        return std::pair<std::string, std::vector<std::string> >(std::move(first), std::move(split_cmd));
    }
}

static void commit_command(shared_command_queue& cmd_queue,
                           std::string const& cmd) {
    auto processed_command = process_command(cmd);
    if (!processed_command.first.empty()) {
        cmd::raw_command c;
        c.args_ = std::move(processed_command.second);
        c.cmd_ = std::move(processed_command.first);
        c.prio_ = cmd::CommandPriority::MEDIUM;
        auto [cmd_q_rw, lock] = cmd_queue.get_payload();
        cmd_q_rw.get().emplace(std::move(c));
    }
}

void user_input_main(shared_command_queue& cmd_queue) {
    using namespace std::chrono_literals;
    // We will take care of termination handling later
    while (true) {
        auto cmd = get_command();
        commit_command(cmd_queue, cmd);
        std::this_thread::sleep_for(10ms);
    }
}
