#pragma once

#include <thread>
#include <mutex>

// convenient variable+mutex wrapper

template <typename T>
struct LockedItem {
    T value;
    std::mutex mutex;
    LockedItem(T init = T()) :value(init) {};
    ~LockedItem() {};
};

typedef std::lock_guard<std::mutex> LockedItemGuard;

