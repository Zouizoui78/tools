#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <ranges>
#include <thread>

#include "AService.hpp"

namespace tools {

class ThreadPool : public AService {
public:
    ThreadPool(int thread_count = std::thread::hardware_concurrency());

    ~ThreadPool() noexcept;

    ThreadPool(const ThreadPool &other) = delete;
    ThreadPool &operator=(const ThreadPool &other) = delete;

    ThreadPool(ThreadPool &&other) = delete;
    ThreadPool &operator=(ThreadPool &&other) = delete;

    using Task = std::move_only_function<void()>;

    // Starts threads waiting for tasks.
    // Does nothing is the thread pool is already running (state is STARTING or
    // RUNNING).
    void start() override;

    // Stops the threads created by start().
    // Does nothing if the thread pool is not running (state is STOPPING or
    // STOPPED). This function does not clear the task queue. It also doesn't
    // wait for all the tasks in the queue to be processed, so make sure to call
    // wait() if you need all enqueued tasks to be done before stopping.
    void stop() override;

    template <std::invocable F>
    auto enqueue(F &&f) {
        // "decltype(f())" returns the return type of f.
        // We need the parenthesis after "decltype(f())" because
        // the template argument of packaged_task expects a function type,
        // like e.g. std::function does : std::function<void()>.
        // So for instance if f returns an int,
        // task's type will end up being "std::package_task<int()>".
        auto task = std::packaged_task<decltype(f())()>(std::forward<F>(f));
        auto future = task.get_future();

        {
            std::scoped_lock lock(_mutex);

            // The lambda must be mutable because calling task modifies its
            // state by updating the associated std::future.
            _tasks.emplace([task = std::move(task)]() mutable {
                task();
            });
        }

        _tasks_cv.notify_one();
        return future;
    }

    // Blocks until all the enqueued tasks are completed.
    // Does nothing if the pool is not running as it would block forever if the
    // task queue is not empty.
    void wait() const;

private:
    void thread_loop();

    int _thread_count;

    std::vector<std::jthread> _threads;
    std::atomic<int> _active_tasks = 0;

    std::queue<Task> _tasks;
    mutable std::mutex _mutex;
    mutable std::condition_variable _tasks_cv;
};

} // namespace tools

#endif // THREAD_POOL_HPP
