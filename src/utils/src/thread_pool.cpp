#include "tools/utils/thread_pool.hpp"

namespace tools {

ThreadPool::ThreadPool(int thread_count) {
    start(thread_count);
}

ThreadPool::~ThreadPool() noexcept {
    stop();
}

void ThreadPool::start(int thread_count) {
    if (thread_count < 1) {
        return;
    }

    {
        std::scoped_lock lock(_mutex);

        if (_running) {
            return;
        }

        _running = true;

        for (int i = 0; i < thread_count; ++i) {
            _threads.emplace_back(&ThreadPool::thread_loop, this);
        }
    }
}

void ThreadPool::stop(bool wait) {
    if (wait) {
        this->wait();
    }

    {
        std::scoped_lock lock(_mutex);

        if (!_running) {
            return;
        }

        _running = false;
    }

    _tasks_cv.notify_all();
    _threads.clear();
}

void ThreadPool::thread_loop() {
    while (_running) {
        Task task;

        {
            std::unique_lock lock(_mutex);
            _tasks_cv.wait(lock, [this] {
                return !_running || !_tasks.empty();
            });

            if (!_running) {
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

    if (!_running) {
        return;
    }

    _tasks_cv.wait(lock, [this] {
        return _tasks.empty() && _active_tasks == 0;
    });
}

} // namespace tools