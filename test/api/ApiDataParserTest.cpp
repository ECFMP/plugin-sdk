#include "api/ApiDataParser.h"
#include "api/ApiDataDownloadedEvent.h"
#include "api/EventDataParserInterface.h"
#include "api/FlightInformationRegionDataParserInterface.h"
#include "api/FlowMeasureDataParserInterface.h"
#include "api/InternalApiElementCollection.h"
#include "api/InternalElementCollectionTypes.h"
#include "log/NullLogger.h"
#include "gmock/gmock.h"
#include <ECFMP/event/Event.h>
#include <ECFMP/flightinformationregion/FlightInformationRegion.h>
#include <ECFMP/flowmeasure/FlowMeasure.h>

namespace ECFMPTest::Api {
    class MockEventDataParser : public ECFMP::Api::EventDataParserInterface
    {
        public:
        MOCK_METHOD(
                std::shared_ptr<ECFMP::Api::InternalEventCollection>, ParseEvents,
                (const nlohmann::json&, const ECFMP::Api::InternalFlightInformationRegionCollection&), (override)
        );
    };

    class MockFlightInformationRegionParser : public ECFMP::Api::FlightInformationRegionDataParserInterface
    {
        public:
        MOCK_METHOD(
                std::shared_ptr<ECFMP::Api::InternalFlightInformationRegionCollection>, ParseFirs,
                (const nlohmann::json&), (override)
        );
    };

    class MockFlowMeasureParser : public ECFMP::Api::FlowMeasureDataParserInterface
    {
        public:
        MOCK_METHOD(
                std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection>, ParseFlowMeasures,
                (const nlohmann::json&, const ECFMP::Api::InternalEventCollection&,
                 const ECFMP::Api::InternalFlightInformationRegionCollection&),
                (override)
        );
    };

    class ApiDataParserTest : public testing::Test
    {
        public:
        ApiDataParserTest()
        {
            this->eventParser = std::make_shared<MockEventDataParser>();
            this->firParser = std::make_shared<MockFlightInformationRegionParser>();
            this->flowMeasureParser = std::make_shared<MockFlowMeasureParser>();
            this->logger = std::make_shared<ECFMP::Log::NullLogger>();
            this->apiDataParser = std::make_shared<ECFMP::Api::ApiDataParser>(
                    this->eventParser, this->firParser, this->flowMeasureParser, this->logger
            );
        }

        std::shared_ptr<MockEventDataParser> eventParser;
        std::shared_ptr<MockFlightInformationRegionParser> firParser;
        std::shared_ptr<MockFlowMeasureParser> flowMeasureParser;
        std::shared_ptr<ECFMP::Log::Logger> logger;
        std::shared_ptr<ECFMP::Api::ApiDataParser> apiDataParser;
    };

    TEST_F(ApiDataParserTest, ItStopsIfFirParsingFails)
    {
        nlohmann::json data;
        ECFMP::Api::ApiDataDownloadedEvent event{data};

        EXPECT_CALL(*this->firParser, ParseFirs(data)).WillOnce(testing::Return(nullptr));

        // Check other parsers aren't called
        EXPECT_CALL(*this->eventParser, ParseEvents(testing::_, testing::_)).Times(0);
        EXPECT_CALL(*this->flowMeasureParser, ParseFlowMeasures(testing::_, testing::_, testing::_)).Times(0);

        apiDataParser->OnEvent(event);
    }

    TEST_F(ApiDataParserTest, ItStopsIfEventParsingFails)
    {
        nlohmann::json data;
        ECFMP::Api::ApiDataDownloadedEvent event{data};
        auto firs = std::make_shared<ECFMP::Api::InternalFlightInformationRegionCollection>();

        EXPECT_CALL(*this->firParser, ParseFirs(data)).WillOnce(testing::Return(firs));

        EXPECT_CALL(*this->eventParser, ParseEvents(data, testing::Ref(*firs))).WillOnce(testing::Return(nullptr));

        // Check other parsers aren't called
        EXPECT_CALL(*this->flowMeasureParser, ParseFlowMeasures(testing::_, testing::_, testing::_)).Times(0);

        apiDataParser->OnEvent(event);
    }

    TEST_F(ApiDataParserTest, ItCallsAllParsers)
    {
        nlohmann::json data;
        ECFMP::Api::ApiDataDownloadedEvent event{data};
        auto firs = std::make_shared<ECFMP::Api::InternalFlightInformationRegionCollection>();
        auto events = std::make_shared<ECFMP::Api::InternalEventCollection>();
        auto flowMeasures = std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();

        EXPECT_CALL(*this->firParser, ParseFirs(data)).WillOnce(testing::Return(firs));

        EXPECT_CALL(*this->eventParser, ParseEvents(data, testing::Ref(*firs))).WillOnce(testing::Return(events));

        EXPECT_CALL(*this->flowMeasureParser, ParseFlowMeasures(data, testing::Ref(*events), testing::Ref(*firs)))
                .WillOnce(testing::Return(flowMeasures));

        apiDataParser->OnEvent(event);
    }
}// namespace ECFMPTest::Api
