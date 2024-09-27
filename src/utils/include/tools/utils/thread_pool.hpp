#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <ranges>
#include <thread>

namespace tools {

class ThreadPool {
public:
    // Calls start() if start == true.
    ThreadPool(bool start = true);
    ThreadPool(int thread_count);

    ~ThreadPool() noexcept;

    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;

    ThreadPool(ThreadPool&& other) = delete;
    ThreadPool& operator=(ThreadPool&& other) = delete;

    // Is a move_only_function to store capturing lambdas in the internal
    // queue.
    using Task = std::move_only_function<void()>;

    // Starts <thread_count> threads waiting for tasks.
    // Does nothing is the thread pool is already running.
    // Does nothing if thread_count < 1.
    void start(int thread_count = std::thread::hardware_concurrency());

    // Stops the threads created by start().
    // Does nothing if the thread pool is not running.
    // If wait = true, calls wait() before stopping the threads.
    // This function does not clear the task queue.
    void stop(bool wait = false);

    template <std::invocable F>
    auto enqueue(F&& f) -> std::future<decltype(f())> {
        using return_type = decltype(f());
        std::packaged_task<return_type()> task(std::forward<F>(f));
        std::future<return_type> future = task.get_future();

        {
            std::scoped_lock lock(_mutex);

            // The lambda must be mutable because calling task modifies its
            // state by updating the associated std::future.
            _tasks.emplace([task = std::move(task)]() mutable { task(); });
        }

        _tasks_cv.notify_one();
        return future;
    }

    // Blocks until all the enqueued tasks are completed.
    void wait() const;

private:
    void thread_task();

    std::vector<std::jthread> _threads;

    // Set to false by start().
    // Used to stop the threads when set to false.
    std::atomic<bool> _running = false;

    // Keeps track of the currently processing tasks.
    // Used to wait until all pending tasks are done.
    std::atomic<int> _active_tasks = 0;

    std::queue<Task> _tasks;
    mutable std::mutex _mutex;
    mutable std::condition_variable _tasks_cv;
};

} // namespace tools

#endif // THREAD_POOL_HPP