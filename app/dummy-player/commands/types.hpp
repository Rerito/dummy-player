
#pragma once

#include <queue>
#include <string>
#include <vector>

namespace cmd {

enum class CommandPriority {
    LOW=0,
    MEDIUM,
    HIGH,
    CRITICAL
};

enum class MessagePriority {
    DEBUG=0,
    INFO,
    WARNING,
    ERROR
};

struct raw_command {
    std::string cmd_;
    std::vector<std::string> args_;
    CommandPriority prio_;
};


struct ui_message {
    std::string msg_;
    MessagePriority prio_;
};

struct prio_comp {
    template <typename Item>
    bool operator()(Item const& it1, Item const& it2) const {
        return std::less<decltype(it1.prio_)>{}(it1.prio_, it2.prio_);
    }
};

using command_queue = std::priority_queue<raw_command, std::vector<raw_command>, prio_comp>;
using message_queue = std::priority_queue<ui_message, std::vector<ui_message>, prio_comp>;

} // namespace cmd
