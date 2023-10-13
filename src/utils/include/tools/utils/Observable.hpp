#ifndef OBSERVABLE_HPP
#define OBSERVABLE_HPP

#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace tools::utils {

class Observable;

class IObserver {
public:
    virtual ~IObserver() {}

    /**
     * @brief Notify the observer that the passed event has been triggered.
     * @param sender Reference to the sender.
     * @param event_id String telling what the event is.
     */
    virtual void notify(Observable* sender, const std::string& event_id) = 0;
};

/**
 * @brief Observable implementation with a simple events mechanism.
 */
class Observable {
public:
    virtual ~Observable() {}

    /**
     * @brief Tell whether the passed object has subscribed to the given
     * event or not.
     * @param observer Observer to search for in event's subscribers.
     * @param event_id Event to search observer in.
     * @return subscribed or not. Return false if no event has been
     * registered, if event_id has been registered and if the event_id is
     * invalid.
     */
    bool is_observer(IObserver* observer, const std::string& event_id);

    /**
     * @brief Tell whether the given event_id has subscribers or not.
     * @param event_id
     * @return true if the given event has at least one subscriber.
     */
    bool has_observers(const std::string& event_id);

    /**
     * @brief Return the number of observers for the given event_id.
     * @param event_id Event ID.
     * @return Number of observers.
     */
    uint16_t count_observers(const std::string& event_id);

    /**
     * @brief Register an observer as a new listener of the given event.
     * @param observer New observer.
     * @param event_id ID of the event to subscribe to.
     * @return true : no error ; false : already subscribed
     */
    bool subscribe(IObserver* observer, const std::string& event_id);

    /**
     * @brief Remove an observer from the listeners of the given event.
     * @param observer Observer to remove.
     * @param event_id Event ID. If default value ("") is passed, the
     * observer is unsubscribed from every currently existing events.
     * @return true : no error ; false : not an observer of the given event.
     */
    bool unsubscribe(IObserver* observer, const std::string& event_id = "");

    /**
     * @brief Tell the subscribed observers that the given event has been
     * triggered.
     * @param event_id Event ID.
     */
    void notify_observers(const std::string& event_id);

private:
    /**
     * @brief Tell whether the given event_id exists in the map.
     * @param event_id
     * @return true if the given event exist in the map.
     */
    bool is_event(const std::string& event_id);

    std::map<std::string, std::vector<IObserver*>> _map_subscribers;
    std::mutex _subs_mutex;
};

} // namespace tools::utils

#endif // OBSERVABLE_HPP