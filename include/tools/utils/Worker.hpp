#ifndef WORKER_HPP
#define WORKER_HPP

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>

namespace tools::utils {

/**
 * @brief The Worker class runs a task in the background.
 * The task is started when start() is called and is repeated until stop() is called.
 * The callback can be changed while the task is running. A mutex is used to change the internal std::function object when it is not called.
 */
class Worker {
    public:
    
    Worker();
    Worker(std::function<void ()> task, bool threaded = true);
    ~Worker();

    /**
     * @brief Start the task. A new thread is created and the task starts running in this thread.
     * @return true => successfully started or already running ; false => invalid callback.
     */
    bool start();

    /**
     * @brief Stop and join the task thread.
     */
    void stop();

    /**
     * @brief Tell whether the task is running or not (thread safe).
     * @return true => task is running ; false => task is not running.
     */
    bool is_running();

    /**
     * @brief Set a new task. A mutex is used to avoid changing the callback while it is called.
     * @param task New task.
     */
    bool set_task(std::function<void ()> task);

    /**
     * @brief Set the time to wait between two executions of the task.
     * @param delay
     */
    void set_delay_ms(double delay_ms);

    /**
     * @brief Set the frequency at which the worker runs its task. Cannot be greater than 1MHz. If it is, the function treats the argument as 1MHz.
     * 
     * @param frequency Worker's frequency.
     */
    void set_frequency(uint32_t frequency);

    /**
     * @brief Tell the worker to schedule its task with a high precision, most likely consuming 100% of its cpu core's time.
     * 
     * @param high_precision 
     */
    void set_high_precision(bool high_precision);

    private:
    void task_wrapper();

    std::atomic<bool> _running = false;
    std::function<void ()> _task;
    std::mutex _task_mutex;
    std::thread _thread;
    bool _threaded;
    std::atomic<double> _delay_us = 0;
    std::atomic<double> _sleep_delay_us = 0;

    std::atomic<bool> _high_precision = false;
};

} // namespace tools::utils

#endif // WORKER_HPP