
#include <thread>
#include <iostream>
#include <string>

#include "status_thread.hpp"

using namespace std::chrono_literals;

static auto get_message(shared_message_queue& msg_queue) {
    while (true) {
        {
            auto [qrw, lock] = msg_queue.get_payload();
            auto& q = qrw.get();
            if (!q.empty()) {
                auto msg = q.top();
                q.pop();
                return msg;
            }
        } // lock is destroyed, freeing the queue
        std::this_thread::sleep_for(10ms);
    }
}

static void print_current_ui_state(ui_shared_state const& ui_state) {
    auto [uist_rw, lock] = ui_state.get_payload();
    std::cout << std::endl << "> " << uist_rw.get();
}

static void print_message(ui_shared_state const& ui_state, cmd::ui_message const& msg) {
    std::cout << "> " << msg.msg_ << "\n";
    print_current_ui_state(ui_state);
}

void ui_status_main(shared_message_queue& msg_queue,
                    ui_shared_state const& ui_state) {
    while (true) {
        auto msg = get_message(msg_queue);
        print_message(ui_state, msg);
        std::this_thread::sleep_for(10ms);
    } 
}
