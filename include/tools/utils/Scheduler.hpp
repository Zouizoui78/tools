#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <atomic>
#include <chrono>
#include <functional>
#include <string>

namespace tools::utils {

class Scheduler;

struct Task {
    std::string name;
    std::function<bool ()> task;
    uint64_t delay_us;

    private:
    std::chrono::steady_clock::time_point next_run;

    friend Scheduler;
};

class Scheduler {
    public:
    
    Scheduler();

    ~Scheduler();

    bool add_task(Task task);

    void start();
    void stop();

    inline bool is_running() { return _running; }

    private:

    void loop_wrapper();

    std::atomic<bool> _running = false;
    std::vector<Task> _tasks;
};

} // namespace tools::utils

#endif // SCHEDULER_HPP