#include "tools/utils/observable.hpp"
#include "gtest/gtest.h"

#include <variant>

namespace test {

using namespace tools::utils;

struct IntEvent {
    int value = 0;
};

struct StrEvent {
    std::string value = "";
};

using Event = std::variant<IntEvent, StrEvent>;

// Observer that handles all events
// Events are dispatched to the right handler with std::visit
class Observer : public IObserver<Event> {
public:
    void notify(const Event& event) override {
        std::visit([this](const auto& event) { handle(event); }, event);
    }

    bool test_called = false;

private:
    void handle(const IntEvent& event) {
        test_called = event.value == 1;
    }
    void handle(const StrEvent& event) {
        test_called = event.value == "yes";
    }
};

// Observer for IntEvent only
class IntObserver : public IObserver<Event> {
public:
    void notify(const Event& event) override {
        if (const IntEvent* int_event = std::get_if<IntEvent>(&event)) {
            test_called = int_event->value == 10;
        }
    }

    bool test_called = false;
};

// We cannot use void as an event type
// Here we use std::monostate for data-less events
class DataLessObserver : public IObserver<std::monostate> {
public:
    void notify(const std::monostate& nullevent) override {
        called = true;
    }

    bool called = false;
};

TEST(TestObservable, test_observable_subscription) {
    Observer observer;
    Observable<Event> observable;

    ASSERT_TRUE(observable.add_observer(&observer));
    ASSERT_FALSE(observable.add_observer(&observer));

    ASSERT_TRUE(observable.remove_observer(&observer));
    ASSERT_FALSE(observable.remove_observer(&observer));
}

TEST(TestObservable, test_observable_notify) {
    Observer observer;
    Observable<Event> observable;

    observable.add_observer(&observer);
    ASSERT_FALSE(observer.test_called);

    IntEvent int_e{.value = 0};
    observable.notify_observers(int_e);
    ASSERT_FALSE(observer.test_called);

    int_e.value = 1;
    observable.notify_observers(int_e);
    ASSERT_TRUE(observer.test_called);

    StrEvent str_e{.value = "nope"};
    observable.notify_observers(str_e);
    ASSERT_FALSE(observer.test_called);

    str_e.value = "yes";
    observable.notify_observers(str_e);
    ASSERT_TRUE(observer.test_called);
}

TEST(TestObservable, test_observable_intevent) {
    IntObserver observer;
    Observable<Event> observable;
    observable.add_observer(&observer);
    ASSERT_FALSE(observer.test_called);

    IntEvent e{.value = 10};
    observable.notify_observers(e);

    ASSERT_TRUE(observer.test_called);
}

TEST(TestObservable, test_observable_both_observers) {
    Observable<Event> observable;

    Observer observer;
    IntObserver int_observer;

    observable.add_observer(&observer);
    observable.add_observer(&int_observer);

    IntEvent e{.value = 1};
    observable.notify_observers(e);

    ASSERT_TRUE(observer.test_called);
    ASSERT_FALSE(int_observer.test_called);

    e.value = 10;
    observable.notify_observers(e);

    ASSERT_FALSE(observer.test_called);
    ASSERT_TRUE(int_observer.test_called);

    StrEvent str_event{.value = "yes"};
    observable.notify_observers(str_event);

    ASSERT_TRUE(observer.test_called);
    ASSERT_TRUE(int_observer.test_called);
}

TEST(TestObservable, test_observable_data_less_observer) {
    Observable<std::monostate> observable;
    DataLessObserver observer;
    observable.add_observer(&observer);

    ASSERT_FALSE(observer.called);
    observable.notify_observers({});
    ASSERT_TRUE(observer.called);
}

} // namespace test