#include "tools/utils/Worker.hpp"
#include "tools/utils/Log.hpp"

namespace tools::utils {

static auto logger = new_logger("Worker");

Worker::Worker() {}

Worker::Worker(std::function<void ()> task) {
    _task = task;
}

Worker::~Worker() {
    stop();
}

bool Worker::start() {
    if (_running) {
        logger->warn("Cannot start background task because it is already running.");
        return true;
    }

    if (!_task) {
        logger->error("Cannot start background task, invalid callback. The function passed to the constructor was invalid (empty) or set_task() was called with an empty callback.");
        return false;
    }

    _running = true;
    _thread = std::thread([this]() {
        while (_running) {
            try {
                std::lock_guard lock(_task_mutex);
                _task();
            } catch (const std::bad_function_call &e) {
                logger->error("Failed to run callback.");
            }

            auto now = std::chrono::steady_clock::now();
            auto sleep_end = now + std::chrono::milliseconds(_delay_ms);

            // If the task is not stopped during sleep, keep sleeping until sleep_end.
            while (_running && now < sleep_end) {
                now = std::chrono::steady_clock::now();
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
    });
    return true;
}

void Worker::stop() {
    if (_running) {
        _running = false;
        _thread.join();
    }
}

bool Worker::is_running() {
    return _running;
}

bool Worker::set_task(std::function<void ()> task) {
    if (!task) {
        return false;
    }

    std::lock_guard lock(_task_mutex);
    _task = task;
    return true;
}

void Worker::set_delay_ms(uint32_t delay) {
    _delay_ms = delay;
}

} // namespace tools::utils