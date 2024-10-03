#ifndef MUTEXED_HPP
#define MUTEXED_HPP

#include <mutex>
#include <stdexcept>

namespace tools {

template <typename obj_t, typename mutex_t = std::mutex>
class Mutexed {
    obj_t _obj;
    mutable mutex_t _mutex;

    void check_mutex_ownership() const {
        std::unique_lock lock(_mutex, std::try_to_lock);

        if (lock.owns_lock()) [[unlikely]] {
            throw std::runtime_error(
                "Cannot access mutexed object without locking it first");
        }
    }

public:
    void lock() {
        _mutex.lock();
    }

    void unlock() {
        _mutex.unlock();
    }

    bool try_lock() {
        return _mutex.try_lock();
    }

    obj_t &operator*() {
        check_mutex_ownership();
        return _obj;
    }

    obj_t *operator->() {
        check_mutex_ownership();
        return &_obj;
    }

    obj_t &operator=(const obj_t &other) {
        check_mutex_ownership();

        if (&_obj != &other) {
            _obj = other;
        }

        return _obj;
    }
};

} // namespace tools

#endif