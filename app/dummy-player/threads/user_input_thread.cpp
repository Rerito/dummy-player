
#include <iostream>
#include <thread>

#include "user_input_thread.hpp"

static char read_char() {
    char next_char;
    std::cin.get(next_char);
    return next_char;
}


static void commit_command(shared_command_queue& cmd_queue,
                           std::string const& cmd) {
    
}

static void commit_char(ui_shared_state& ui_state,
                        shared_command_queue& cmd_queue,
                        char c) {
    auto [st, lock] = ui_state.get_payload();
    auto& cur_input = st.get();
    if (c == '\b') {
        // erase the last character
        cur_input.erase(size(cur_input) - 1);
    } else {
        if ('\n' == c) {
            commit_command(cmd_queue, cur_input);
        } else {
            cur_input.push_back(c);
        }
    }
}

void user_input_main(shared_command_queue& cmd_queue,
                     ui_shared_state& ui_state) {
    using namespace std::chrono_literals;
    // We will take care of termination handling later
    while (true) {
        auto c = read_char();
        commit_char(ui_state, cmd_queue, c);
        std::this_thread::sleep_for(10ms);
    }
}
