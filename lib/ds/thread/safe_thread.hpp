
#pragma once

#include <thread>
#include <functional>
#include <exception>


#include "shared_state.hpp"

template <typename Thread = std::thread>
struct safe_thread : public Thread {
    template <typename F>
    safe_thread(F&& ftor, shared_state<std::exception_ptr>& eptr) :
        Thread([&]() {
            try {
                ftor();
            } catch (...) {
                auto [eptr_rw, lock] = eptr.get_payload();
                eptr_rw.get() = std::current_exception();
            }
        }) {}
};
