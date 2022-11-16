#include <gtest/gtest.h>
#include "Observable.hpp"
#include "Log.hpp"

#define EVENT "test"
#define EVENT2 "test2"

namespace test {

using namespace tools::utils;

static auto logger = tools::utils::new_logger("TestObservable");

class DummyObserver : public IObserver {
public:
    virtual ~DummyObserver () {}

    virtual void notify(Observable *sender, const std::string &event_id) override {
        (void)sender;
        if (event_id == EVENT)
            test_called = !test_called;
        else if (event_id == EVENT2)
            test2_called = !test2_called;
    }

    bool test_called = false;
    bool test2_called = false;
};

class TestObservable:   public ::testing::Test
{
    protected:
    TestObservable() {
        outputs_path = std::string(std::getenv("TEST_OUTPUTS"));
    }

    virtual ~TestObservable() {}

    virtual void SetUp() {}

    virtual void TearDown() {}

    public:
    std::string outputs_path;
};

TEST_F(TestObservable, test_getters) {
    DummyObserver observer;
    DummyObserver observer2;
    Observable observable;

    ASSERT_TRUE(observable.subscribe(&observer, EVENT2));
    ASSERT_FALSE(observable.has_observers(EVENT));
    ASSERT_EQ(observable.count_observers(EVENT), 0);

    ASSERT_FALSE(observable.is_observer(&observer, EVENT));
    ASSERT_TRUE(observable.subscribe(&observer, EVENT));
    ASSERT_TRUE(observable.has_observers(EVENT));
    ASSERT_TRUE(observable.is_observer(&observer, EVENT));

    ASSERT_EQ(observable.count_observers(EVENT), 1);
    ASSERT_TRUE(observable.subscribe(&observer2, EVENT));
    ASSERT_EQ(observable.count_observers(EVENT), 2);
}

TEST_F(TestObservable, test_subscription) {
    DummyObserver observer;
    Observable observable;

    ASSERT_FALSE(observable.is_observer(&observer, EVENT));

    ASSERT_TRUE(observable.subscribe(&observer, EVENT));
    ASSERT_TRUE(observable.subscribe(&observer, EVENT2));
    ASSERT_TRUE(observable.is_observer(&observer, EVENT));
    ASSERT_TRUE(observable.is_observer(&observer, EVENT2));

    ASSERT_TRUE(observable.unsubscribe(&observer));
    ASSERT_FALSE(observable.is_observer(&observer, EVENT));
    ASSERT_FALSE(observable.is_observer(&observer, EVENT2));
}

TEST_F(TestObservable, test_notify) {
    DummyObserver observer;
    Observable observable;

    observable.subscribe(&observer, EVENT);
    observable.subscribe(&observer, EVENT2);

    ASSERT_FALSE(observer.test_called);
    ASSERT_FALSE(observer.test2_called);

    observable.notify_observers("not an event");

    // Nothing should have changed.
    ASSERT_FALSE(observer.test_called);
    ASSERT_FALSE(observer.test2_called);

    observable.notify_observers(EVENT);

    ASSERT_TRUE(observer.test_called);
    ASSERT_FALSE(observer.test2_called);

    observable.notify_observers(EVENT2);

    ASSERT_TRUE(observer.test_called);
    ASSERT_TRUE(observer.test2_called);
}

} // namespace test