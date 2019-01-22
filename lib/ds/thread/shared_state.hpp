
#pragma once

#include <mutex>
#include <shared_mutex>
#include <utility>
#include <functional>
#include <optional>

#include "utils/forward_macro.hpp"

template <typename Payload, typename Mutex = std::shared_mutex>
class shared_state {
public:
    // The payload is an optional to take care of non default-constructible types
    using payload_type = std::optional<Payload>;
    using mutex_type = Mutex;
    using const_ref_wrapper = std::reference_wrapper<Payload const>;
    using ref_wrapper = std::reference_wrapper<Payload>;
    using write_lock_type = std::unique_lock<mutex_type>;
    using read_lock_type = std::shared_lock<mutex_type>;

    template <typename... Args>
    shared_state(Args&&... args) :
        payload_(std::in_place_t {}, CPPFWD(args)...),
        mutex_() {}

    shared_state(shared_state const& other) :
        payload_(),
        mutex_() {
        read_lock_type rlock(other.mutex_);
        payload_ = other.payload_;
    }

    shared_state(shared_state&& other) :
        payload_(),
        mutex_() {
        write_lock_type wlock(other.mutex_);
        payload_ = std::move(other.payload_);
    }

    // We cannot use an all-in-one operator= implementation
    // Because we need to lock the 2 shared_state objects simultaneously
    shared_state& operator=(shared_state const& other) {
        if (&other != this) {
            read_lock_type rlock_other(other.mutex_, std::defer_lock);
            write_lock_type wlock(mutex_, std::defer_lock);
            std::lock(rlock_other, wlock);
            payload_ = other.payload_;
        }
        return *this;
    }

    shared_state& operator=(shared_state&& other) {
        if (&other != this) {
            write_lock_type wlock(mutex_, std::defer_lock);
            write_lock_type wlock_other(other.mutex_, std::defer_lock);
            std::lock(wlock, wlock_other); // We must lock the 2 objects at the same time for writing!
            payload_ = std::move(other.payload_);
        }
        return *this;
    }

    std::pair<ref_wrapper, write_lock_type> get_payload() {
        return std::make_pair(std::ref(*payload_), write_lock_type(mutex_));
    }

    std::pair<const_ref_wrapper, read_lock_type> get_payload() const {
        return std::make_pair(std::cref(*payload_), read_lock_type(mutex_));
    }

private:
    payload_type payload_;
    mutable mutex_type mutex_;
};

// Make a copy of the shared state
template <typename T>
T get_shared_state(shared_state<T> const& state) {
    return state.get_payload().first.get();
}

template <typename T, typename F>
auto apply_to_shared_state(shared_state<T>& state, F&& ftor) {
    auto [pl, wlock] = state.get_payload();
    return CPPFWD(ftor)(pl.get());
}

template <typename T, typename F>
auto apply_to_shared_state(shared_state<T> const& state, F&& ftor) {
    auto [pl, rlock] = state.get_payload();
    return CPPFWD(ftor)(pl.get());
}
