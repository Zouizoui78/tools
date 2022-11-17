#include <gtest/gtest.h>
#include "tools/net/HTTPClient.hpp"
#include "tools/utils/Log.hpp"

namespace test {

using namespace tools::net;

class TestHTTPClient:   public ::testing::Test
{
    protected:
        TestHTTPClient() {}

        virtual ~TestHTTPClient() {}

        virtual void SetUp() {}

        virtual void TearDown() {}

    public:
};

std::string expected_google_301 = "<HTML><HEAD><meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n<TITLE>301 Moved</TITLE></HEAD><BODY>\n<H1>301 Moved</H1>\nThe document has moved\n<A HREF=\"https://www.google.com/\">here</A>.\r\n</BODY></HTML>\r\n";

TEST_F(TestHTTPClient, synchronous) {
    HTTPClient client;
    ASSERT_TRUE(client.is_ok());
    HTTPReply reply;

    // We test POST request first to make sure it doesn't break following GET requests
    // since POST requests implicitly change some curl settings.
    EXPECT_TRUE(client.post(reply, "https://google.com", "test", { "Accept: application/json" }));
    EXPECT_EQ(reply.host, "google.com");
    EXPECT_EQ(reply.res_code, 405);

    EXPECT_TRUE(client.get(reply, "https://google.com", { "Accept: application/json" }));
    EXPECT_EQ(reply.host, "google.com");
    EXPECT_EQ(reply.res_code, 301);
    EXPECT_EQ(reply.data, expected_google_301);
}

TEST_F(TestHTTPClient, async) {
    HTTPClient client;
    ASSERT_TRUE(client.is_ok());

    uint8_t callbacks_called = 0;
    std::mutex mutex;

    EXPECT_TRUE(client.post_async("https://google.com", "test", [&](HTTPReply &reply) {
        EXPECT_EQ(reply.host, "google.com");
        EXPECT_EQ(reply.res_code, 405);
        std::lock_guard lock(mutex);
        ++callbacks_called;
    }, { "Accept: application/json" }));

    EXPECT_TRUE(client.get_async("https://google.com", [&](HTTPReply &reply) {
        EXPECT_EQ(reply.host, "google.com");
        EXPECT_EQ(reply.data, expected_google_301);
        EXPECT_EQ(reply.res_code, 301);
        std::lock_guard lock(mutex);
        ++callbacks_called;
    }, { "Accept: application/json" }));

    client.wait_async_requests();

    EXPECT_EQ(callbacks_called, 2);
}

TEST_F(TestHTTPClient, multiple_clients) {
    std::vector<std::unique_ptr<HTTPClient>> clients;

    uint8_t expected_callbacks_called = 5;
    uint8_t callbacks_called = 0;
    std::mutex mutex;

    for (int i = 0 ; i < expected_callbacks_called ; ++i) {
        std::unique_ptr<HTTPClient> client = std::make_unique<HTTPClient>();
        ASSERT_TRUE(client->is_ok());
        EXPECT_TRUE(client->get_async("https://google.com", [&](HTTPReply &reply) {
            EXPECT_EQ(reply.host, "google.com");
            EXPECT_EQ(reply.data, expected_google_301);
            EXPECT_EQ(reply.res_code, 301);
            std::lock_guard lock(mutex);
            ++callbacks_called;
        }, { "Accept: application/json" }));
        clients.push_back(std::move(client));
    }

    for (auto &client : clients)
        client->wait_async_requests();

    EXPECT_EQ(callbacks_called, expected_callbacks_called);
}

TEST_F(TestHTTPClient, failures) {
    SPDLOG_INFO("Errors are expected in this test.");
    HTTPClient client;
    ASSERT_TRUE(client.is_ok());
    HTTPReply reply;

    EXPECT_FALSE(client.get(reply, ""));

    EXPECT_FALSE(client.post(reply, "", "test"));
    EXPECT_FALSE(client.post(reply, "http://google.com", ""));

    EXPECT_FALSE(client.get_async("", [](HTTPReply &reply){ (void)reply; }));

    EXPECT_FALSE(client.post_async("", "test", [](HTTPReply &reply){ (void)reply; }));
    EXPECT_FALSE(client.post_async("https://google.com", "", [](HTTPReply &reply){ (void)reply; }));
}

TEST_F(TestHTTPClient, nested) {
    HTTPClient client;
    ASSERT_TRUE(client.is_ok());

    uint16_t count = 0;
    std::mutex mutex;

    for (int i = 0 ; i < 10 ; ++i) {
        bool res = client.get_async("google.com", [&client, &count, &mutex](HTTPReply &reply) {
            (void)reply;
            if (client.get(reply, "google.com")) {
                {
                    std::lock_guard lock(mutex);
                    ++count;
                }
            }
        });
        EXPECT_TRUE(res);
    }
    client.wait_async_requests();

    EXPECT_EQ(count, 10);
}

TEST_F(TestHTTPClient, early_dtor) {
    HTTPClient *client = new HTTPClient;
    ASSERT_TRUE(client->is_ok());

    client->get_async("google.com", [](HTTPReply &reply) {
        (void)reply;
    });

    EXPECT_NO_THROW(delete client);
}

} // namespace test