#include "api/ApiDataDownloader.h"
#include "ECFMP/http/HttpClient.h"
#include "api/ApiDataDownloadedEvent.h"
#include "mock/MockHttpClient.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"

namespace ECFMPTest::Api {

    class MockEventHandler : public ECFMP::EventBus::NewEventListener<ECFMP::Api::ApiDataDownloadedEvent>
    {
        public:
        explicit MockEventHandler(nlohmann::json expectedJson) : expectedJson(std::move(expectedJson))
        {}

        void OnEvent(const ECFMP::Api::ApiDataDownloadedEvent& event) override
        {
            callCount++;
            ASSERT_EQ(event.data, expectedJson);
        }

        [[nodiscard]] auto GetCallCount() const -> int
        {
            return callCount;
        }

        private:
        nlohmann::json expectedJson;

        int callCount = 0;
    };

    class ApiDataDownloaderTest : public testing::Test
    {
        public:
        ApiDataDownloaderTest()
            : mockEventHandler(std::make_shared<MockEventHandler>(nlohmann::json{{"abc", "def"}})),
              eventBus(std::make_shared<ECFMP::EventBus::InternalEventBus>()),
              logger(std::make_shared<testing::NiceMock<Log::MockLogger>>()),
              httpClient(std::make_unique<testing::NiceMock<Http::MockHttpClient>>())
        {
            // Add mock listener to event bus
            eventBus->Subscribe<ECFMP::Api::ApiDataDownloadedEvent>(mockEventHandler);
        }

        [[nodiscard]] auto MakeDownloader() -> ECFMP::Api::ApiDataDownloader
        {
            return ECFMP::Api::ApiDataDownloader(std::move(httpClient), eventBus, logger);
        }

        const std::string API_URL = "https://ecfmp.vatsim.net/api/v1/plugin?deleted=1";

        std::shared_ptr<MockEventHandler> mockEventHandler;
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<testing::NiceMock<Log::MockLogger>> logger;
        std::unique_ptr<testing::NiceMock<Http::MockHttpClient>> httpClient;
    };

    TEST_F(ApiDataDownloaderTest, ItDoesNothingIfApiReturnsNonOkCode)
    {
        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{400L, "abc"}));

        EXPECT_CALL(*logger, Error).Times(1);

        MakeDownloader().DownloadData();

        EXPECT_EQ(0, mockEventHandler->GetCallCount());
    }

    TEST_F(ApiDataDownloaderTest, ItDoesNothingIfApiReturnsInvalidJson)
    {
        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{200L, "{]"}));

        EXPECT_CALL(*logger, Error).Times(1);

        MakeDownloader().DownloadData();

        EXPECT_EQ(0, mockEventHandler->GetCallCount());
    }

    TEST_F(ApiDataDownloaderTest, ItDoesNothingIfApiReturnsNonObjectJson)
    {
        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{200L, "[]"}));

        EXPECT_CALL(*logger, Error).Times(1);

        MakeDownloader().DownloadData();

        EXPECT_EQ(0, mockEventHandler->GetCallCount());
    }

    TEST_F(ApiDataDownloaderTest, ItCallsListenersWithData)
    {
        nlohmann::json data = {{"abc", "def"}};

        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{200L, data.dump()}));

        EXPECT_CALL(*logger, Error).Times(0);

        MakeDownloader().DownloadData();

        eventBus->ProcessPendingEvents();
        EXPECT_EQ(1, mockEventHandler->GetCallCount());
    }
}// namespace ECFMPTest::Api
