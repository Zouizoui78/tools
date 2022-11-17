#include "tools/utils/Scheduler.hpp"
#include "tools/utils/Log.hpp"

#include <thread>

namespace tools::utils {

Scheduler::Scheduler() {}
    
Scheduler::~Scheduler() {}

bool Scheduler::add_task(Task task) {
    if (is_running() || task.name.empty() || !task.task)
        return false;

    _tasks.push_back(task);
    return true;
}

void Scheduler::start() {
    auto now = std::chrono::steady_clock::now();
    for (auto &e : _tasks)
        e.next_run = now + std::chrono::microseconds(e.delay_us);
    _running = true;
    loop_wrapper();
}

void Scheduler::stop() {
    _running = false;
}

void Scheduler::loop_wrapper() {
    while (is_running()) {
        for (auto &e : _tasks) {
            if (!is_running())
                break;
            
            auto now = std::chrono::steady_clock::now();
            if (now > e.next_run) {
                if (!e.task())
                    SPDLOG_ERROR("Task '{}' returned false.", e.name);
                e.next_run = now + std::chrono::microseconds(e.delay_us);
            }
        }
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }
}

} // namespace tools::utils