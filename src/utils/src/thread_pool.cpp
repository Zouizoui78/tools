#include "tools/utils/thread_pool.hpp"

namespace tools::utils {

ThreadPool::ThreadPool(int thread_count) : _thread_count(thread_count) {
    start();
}

ThreadPool::~ThreadPool() noexcept {
    stop();
}

ServiceState ThreadPool::get_state() const {
    return _state;
}

void ThreadPool::start() {
    {
        std::scoped_lock lock(_mutex);

        if (_state == ServiceState::RUNNING) {
            return;
        }

        _state = ServiceState::RUNNING;

        for (int i = 0; i < _thread_count; ++i) {
            _threads.emplace_back(&ThreadPool::thread_loop, this);
        }
    }
}

void ThreadPool::stop() {
    {
        std::scoped_lock lock(_mutex);

        if (_state == ServiceState::STOPPED) {
            return;
        }

        _state = ServiceState::STOPPED;
    }

    _tasks_cv.notify_all();
    _threads.clear();
}

void ThreadPool::thread_loop() {
    while (_state == ServiceState::RUNNING) {
        Task task;

        {
            std::unique_lock lock(_mutex);
            _tasks_cv.wait(lock, [this] {
                return _state == ServiceState::STOPPED || !_tasks.empty();
            });

            if (_state == ServiceState::STOPPED) {
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

    if (_state == ServiceState::STOPPED) {
        return;
    }

    _tasks_cv.wait(lock, [this] {
        return _tasks.empty() && _active_tasks == 0;
    });
}

} // namespace tools::utils