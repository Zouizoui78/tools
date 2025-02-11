#include "tools/misc/ThreadPool.hpp"

namespace tools {

ThreadPool::ThreadPool(int thread_count) : _thread_count(thread_count) {
    start();
}

ThreadPool::~ThreadPool() noexcept {
    stop();
}

void ThreadPool::start() {
    if (this->is_running_or_starting()) {
        return;
    }

    _state = ServiceState::STARTING;

    for (int i = 0; i < _thread_count; ++i) {
        _threads.emplace_back(&ThreadPool::thread_loop, this);
    }

    _state = ServiceState::RUNNING;
}

void ThreadPool::stop() {
    if (this->is_stopped_or_stopping()) {
        return;
    }

    _state = ServiceState::STOPPING;

    _tasks_cv.notify_all();
    _threads.clear();

    _state = ServiceState::STOPPED;
}

void ThreadPool::thread_loop() {
    Task task;

    while (this->is_running_or_starting()) {

        {
            std::unique_lock lock(_mutex);
            _tasks_cv.wait(lock, [this] {
                return this->is_stopped_or_stopping() || !_tasks.empty();
            });

            if (this->is_stopped_or_stopping()) {
                return;
            }

            task = std::move(_tasks.front());
            _tasks.pop();

            // Must be incremented when the mutex is locked.
            // Otherwise wait() can return between the release of the mutex and
            // the incrementation of this counter.
            ++_active_tasks;
        }

        task();
        --_active_tasks;
        _tasks_cv.notify_all();
    }
}

void ThreadPool::wait() const {
    std::unique_lock lock(_mutex);

    if (this->is_stopped_or_stopping()) {
        return;
    }

    _tasks_cv.wait(lock, [this] {
        return _tasks.empty() && _active_tasks == 0;
    });
}

} // namespace tools