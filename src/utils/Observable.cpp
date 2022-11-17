#include "tools/utils/Observable.hpp"
#include "tools/utils/Log.hpp"

namespace tools::utils {

bool Observable::is_event(const std::string &event_id) {
    std::lock_guard lock(_subs_mutex);
    return _map_subscribers.find(event_id) != _map_subscribers.end();
}

bool Observable::has_observers(const std::string &event_id) {
    if (!is_event(event_id))
        return false;
    return count_observers(event_id) > 0;
}

bool Observable::is_observer(IObserver *observer, const std::string &event_id) {
    {
        std::lock_guard lock(_subs_mutex);
        if (_map_subscribers.empty())
            return false;
    }

    if (event_id.empty() || !is_event(event_id))
        return false;

    {
        std::lock_guard lock(_subs_mutex);
        auto &subs = _map_subscribers[event_id];
        return std::find(subs.begin(), subs.end(), observer) != subs.end();
    }
}

uint16_t Observable::count_observers(const std::string &event_id) {
    if (event_id.empty() || !is_event(event_id))
        return 0;

    std::lock_guard lock(_subs_mutex);
    return _map_subscribers[event_id].size();
}

bool Observable::subscribe(IObserver *observer, const std::string &event_id) {
    if (event_id.empty()) {
        for (const auto &e : _map_subscribers) {
            subscribe(observer, e.first);
        }
        return true;
    }

    if (is_observer(observer, event_id))
        return false;

    std::lock_guard lock(_subs_mutex);
    _map_subscribers[event_id].push_back(observer);
    return true;
}

bool Observable::unsubscribe(IObserver *observer, const std::string &event_id) {
    if (event_id.empty()) {
        for (const auto &e : _map_subscribers) {
            unsubscribe(observer, e.first);
        }
        return true;
    }

    if (!is_event(event_id) || !is_observer(observer, event_id))
        return false;

    std::lock_guard lock(_subs_mutex);
    auto &subs = _map_subscribers[event_id];
    auto it = std::find(subs.begin(), subs.end(), observer);
    subs.erase(it);

    return true;
}

void Observable::notify_observers(const std::string &event_id) {
    if (!is_event(event_id))
        return;

    std::lock_guard lock(_subs_mutex);
    for (const auto &e : _map_subscribers[event_id])
        e->notify(this, event_id);
}

} // namespace tools::utils