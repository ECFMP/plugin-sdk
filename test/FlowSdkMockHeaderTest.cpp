#include "mock/flow-sdk-mock.h"

namespace FlowSdkTest {
    class FlowSdkHeaderMockTest : public testing::Test
    {
    };

    TEST_F(FlowSdkHeaderMockTest, ItLoadsHeader)
    {
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::AirportFilterMock> airportFilter;
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::EventFilterMock> eventFilter;
        testing::NiceMock<FlowSdk::Mock::Event::EventMock> event;
        testing::NiceMock<FlowSdk::Mock::Event::EventParticipantMock> participant;
        testing::NiceMock<FlowSdk::Mock::FlightInformationRegion::FlightInformationRegionMock> fir;
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::FlowMeasureMock> flowMeasure;
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::FlowMeasureFiltersMock> filters;
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::LevelRangeFilterMock> levelFilter;
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::MeasureMock> measure;
        testing::NiceMock<FlowSdk::Mock::FlowMeasure::RouteFilterMock> routeFilter;

        // Dummy expectation, the purpose of this test is to make sure the header compiles properly.
        EXPECT_TRUE(true);
    }
}// namespace FlowSdkTest
