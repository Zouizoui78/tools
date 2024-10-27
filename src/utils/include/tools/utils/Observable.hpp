#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include <algorithm>
#include <mutex>
#include <vector>

namespace tools::utils {

template <typename Event>
class IObserver {
public:
    virtual ~IObserver() noexcept = default;

    IObserver(const IObserver &other) = delete;
    IObserver(IObserver &&other) = delete;
    IObserver &operator=(const IObserver &other) = delete;
    IObserver &operator=(IObserver &&other) = delete;

    IObserver() = default;

    virtual void notify(const Event &event) = 0;
};

template <typename Event>
class Observable final {
private:
    std::vector<IObserver<Event> *> _observers;
    std::mutex _observers_mutex;

    std::vector<IObserver<Event> *> _observers_to_remove;
    std::mutex _observers_to_remove_mutex;

public:
    void notify_observers(const Event &event) {
        std::scoped_lock observer_lock(_observers_mutex);

        {
            std::scoped_lock to_remove_lock(_observers_to_remove_mutex);

            std::erase_if(_observers, [this](IObserver<Event> *current_obs) {
                return std::ranges::contains(_observers_to_remove, current_obs);
            });

            _observers_to_remove.clear();
        }

        std::ranges::for_each(_observers, [&event](IObserver<Event> *observer) {
            observer->notify(event);
        });
    }

    void add_observer(IObserver<Event> *observer) {
        std::scoped_lock lock(_observers_mutex);
        _observers.push_back(observer);
    }

    void remove_observer(IObserver<Event> *observer) {
        std::scoped_lock lock_to_remove(_observers_to_remove_mutex);
        _observers_to_remove.emplace_back(observer);
    };
};

} // namespace tools::utils

#endif // OBSERVABLE_HPP