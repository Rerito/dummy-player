
#include <thread>
#include <iostream>
#include <string>

#include "status_thread.hpp"

using namespace std::chrono_literals;

static auto get_message(shared_message_queue& msg_queue) {
    while (true) {
        auto msg = pop_shared_queue(msg_queue);
        if (msg) {
           return std::move(*msg);
        }
        std::this_thread::sleep_for(10ms);
    }
}

static void print_message(cmd::ui_message const& msg) {
    std::cout << "> " << msg.msg_ << "\n";
}

void ui_status_main(shared_message_queue& msg_queue) {
    while (true) {
        auto msg = get_message(msg_queue);
        print_message(msg);
        std::this_thread::sleep_for(10ms);
    } 
}
