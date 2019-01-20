
#include <iostream>
#include <thread>

#include "user_input_thread.hpp"

static std::string get_command() {
    std::string cmd;
    std::getline(std::cin, cmd);
    return cmd;
}


static void commit_command(shared_command_queue& cmd_queue,
                           std::string const& cmd) {
}

void user_input_main(shared_command_queue& cmd_queue,
                     ui_shared_state& /*ui_state*/) {
    using namespace std::chrono_literals;
    // We will take care of termination handling later
    std:: cout << "> ";
    while (true) {
        auto cmd = get_command();
        commit_command(cmd_queue, cmd);
        std::this_thread::sleep_for(10ms);
    }
}
