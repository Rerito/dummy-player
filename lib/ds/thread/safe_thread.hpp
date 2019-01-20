
#pragma once

#include <thread>
#include <functional>
#include <exception>


#include "shared_state.hpp"

struct safe_thread : public std::thread {
    template <typename F>
    safe_thread(F&& ftor, shared_state<std::exception_ptr>& eptr) :
        std::thread([&]() {
            try {
                ftor();
            } catch (...) {
                auto [eptr_rw, lock] = eptr.get_payload();
                eptr_rw.get() = std::current_exception();
            }
        }) {}
};
