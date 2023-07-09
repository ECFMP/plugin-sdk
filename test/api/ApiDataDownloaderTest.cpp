#include "api/ApiDataDownloader.h"
#include "ECFMP/http/HttpClient.h"
#include "mock/MockEventListener.h"
#include "mock/MockHttpClient.h"
#include "mock/MockLogger.h"
#include "nlohmann/json.hpp"
#include "plugin/ConcreteEventListeners.h"

namespace ECFMPTest::Api {
    class ApiDataDownloaderTest : public testing::Test
    {
        public:
        ApiDataDownloaderTest()
            : logger(std::make_shared<testing::NiceMock<Log::MockLogger>>()),
              httpClient(std::make_unique<testing::NiceMock<Http::MockHttpClient>>()),
              listener(std::make_unique<testing::NiceMock<Plugin::MockEventListener<const nlohmann::json&>>>())
        {}

        [[nodiscard]] auto MakeDownloader() -> ECFMP::Api::ApiDataDownloader
        {
            auto listeners = std::make_unique<ECFMP::Plugin::ConcreteEventListeners<const nlohmann::json&>>();
            listeners->Add(listener);

            return ECFMP::Api::ApiDataDownloader(std::move(httpClient), std::move(listeners), logger);
        }

        const std::string API_URL = "https://ecfmp.vatsim.net/api/v1/plugin?deleted=1";

        std::shared_ptr<testing::NiceMock<Log::MockLogger>> logger;
        std::unique_ptr<testing::NiceMock<Http::MockHttpClient>> httpClient;
        std::shared_ptr<testing::NiceMock<Plugin::MockEventListener<const nlohmann::json&>>> listener;
    };

    TEST_F(ApiDataDownloaderTest, ItDoesNothingIfApiReturnsNonOkCode)
    {
        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{400L, "abc"}));

        EXPECT_CALL(*logger, Error).Times(1);

        EXPECT_CALL(*listener, OnEvent).Times(0);

        MakeDownloader().DownloadData();
    }

    TEST_F(ApiDataDownloaderTest, ItDoesNothingIfApiReturnsInvalidJson)
    {
        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{200L, "{]"}));

        EXPECT_CALL(*logger, Error).Times(1);

        EXPECT_CALL(*listener, OnEvent).Times(0);

        MakeDownloader().DownloadData();
    }

    TEST_F(ApiDataDownloaderTest, ItDoesNothingIfApiReturnsNonObjectJson)
    {
        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{200L, "[]"}));

        EXPECT_CALL(*logger, Error).Times(1);

        EXPECT_CALL(*listener, OnEvent).Times(0);

        MakeDownloader().DownloadData();
    }

    TEST_F(ApiDataDownloaderTest, ItCallsListenersWithData)
    {
        nlohmann::json data = {{"abc", "def"}};

        EXPECT_CALL(*httpClient.get(), Get(API_URL))
                .Times(1)
                .WillOnce(testing::Return(ECFMP::Http::HttpResponse{200L, data.dump()}));

        EXPECT_CALL(*logger, Error).Times(0);

        EXPECT_CALL(*listener, OnEvent(data)).Times(1);

        MakeDownloader().DownloadData();
    }
}// namespace ECFMPTest::Api
