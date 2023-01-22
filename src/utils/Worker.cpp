#include "tools/utils/Worker.hpp"
#include "tools/utils/Log.hpp"

namespace tools::utils {

Worker::Worker() {}

Worker::Worker(std::function<void ()> task, bool threaded) {
    _task = task;
    _threaded = threaded;
}

Worker::~Worker() {
    stop();
}

bool Worker::start() {
    if (_running) {
        SPDLOG_WARN("Cannot start background task because it is already running.");
        return true;
    }

    if (!_task) {
        SPDLOG_ERROR("Cannot start background task, invalid callback. The function passed to the constructor was invalid (empty) or set_task() was called with an empty callback.");
        return false;
    }

    _running = true;

    if (_threaded)
        _thread = std::thread([this]() {
            task_wrapper();
        });
    else
        task_wrapper();

    return true;
}

void Worker::stop() {
    if (_running) {
        _running = false;

        if (_threaded)
            _thread.join();
    }
}

bool Worker::is_running() {
    return _running;
}

void Worker::task_wrapper() {
    while (_running) {
        std::chrono::steady_clock::time_point sleep_end;

        if (!_high_precision)
            sleep_end = std::chrono::steady_clock::now() + std::chrono::microseconds(_sleep_delay_us);

        auto wait_end = std::chrono::steady_clock::now() + std::chrono::microseconds(_delay_us);

        try {
            std::lock_guard lock(_task_mutex);
            _task();
        } catch (const std::bad_function_call &e) {
            SPDLOG_ERROR("Failed to run callback : bad_function_call.");
        }

        // If the task is not stopped during sleep, keep sleeping until sleep_end.
        // We don't sleep for the whole wait duration
        // to minimize inaccuracy caused by the OS' scheduler.
        while (!_high_precision && _running && std::chrono::steady_clock::now() < sleep_end) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }

        // Now we "actively wait" until
        // the end of the wait time.
        while (_running && std::chrono::steady_clock::now() < wait_end);
    }
}

bool Worker::set_task(std::function<void ()> task) {
    if (!task) {
        return false;
    }

    std::lock_guard lock(_task_mutex);
    _task = task;
    return true;
}

void Worker::set_delay_ms(double delay_ms) {
    _delay_us = delay_ms * 1000;
    _sleep_delay_us = _delay_us * 0.9;
}

void Worker::set_frequency(uint32_t frequency) {
    if (frequency > 1e6) {
        SPDLOG_WARN("Trying to set frequency to {}, which is to high. Setting frequency to the maximum (1MHz).", frequency);
        frequency = 1e6;
    }
    set_delay_ms(1000.0 / frequency);
}

void Worker::set_high_precision(bool high_precison) {
    _high_precision = high_precison;
}

} // namespace tools::utils