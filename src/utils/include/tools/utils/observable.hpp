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

    IObserver(const IObserver& other) = delete;
    IObserver(IObserver&& other) = delete;
    IObserver& operator=(const IObserver& other) = delete;
    IObserver& operator=(IObserver&& other) = delete;

    IObserver() = default;

    virtual void notify(const Event& event) = 0;
};

// Observer pattern implementation.
// References to observers are saved instead of callbacks to be able to
// unsubscribe (remove) them.
template <typename Event>
class Observable final {
private:
    std::vector<IObserver<Event>*> _observers;
    std::mutex _observers_mutex;

public:
    void notify_observers(const Event& event) {
        std::lock_guard lock(_observers_mutex);
        std::ranges::for_each(_observers, [&event](IObserver<Event>* observer) {
            observer->notify(event);
        });
    }

    // Return false if the observer is already registered.
    bool add_observer(IObserver<Event>* observer) {
        std::lock_guard lock(_observers_mutex);
        if (std::ranges::find(_observers, observer) != _observers.end()) {
            return false;
        }
        _observers.push_back(observer);
        return true;
    }

    // Return false if the observer is not found.
    bool remove_observer(IObserver<Event>* observer) {
        std::lock_guard lock(_observers_mutex);
        if (auto it = std::ranges::find(_observers, observer);
            it != _observers.end()) {
            _observers.erase(it);
            return true;
        }
        return false;
    };
};

} // namespace tools::utils

#endif // OBSERVABLE_HPP