#include "mock/flow-sdk-mock.h"

namespace ECFMPTest {
    class FlowSdkHeaderMockTest : public testing::Test
    {
    };

    TEST_F(FlowSdkHeaderMockTest, ItLoadsHeader)
    {
        testing::NiceMock<ECFMP::Mock::FlowMeasure::AirportFilterMock> airportFilter;
        testing::NiceMock<ECFMP::Mock::FlowMeasure::EventFilterMock> eventFilter;
        testing::NiceMock<ECFMP::Mock::Event::EventMock> event;
        testing::NiceMock<ECFMP::Mock::Event::EventParticipantMock> participant;
        testing::NiceMock<ECFMP::Mock::FlightInformationRegion::FlightInformationRegionMock> fir;
        testing::NiceMock<ECFMP::Mock::FlowMeasure::FlowMeasureMock> flowMeasure;
        testing::NiceMock<ECFMP::Mock::FlowMeasure::FlowMeasureFiltersMock> filters;
        testing::NiceMock<ECFMP::Mock::FlowMeasure::LevelRangeFilterMock> levelFilter;
        testing::NiceMock<ECFMP::Mock::FlowMeasure::MeasureMock> measure;
        testing::NiceMock<ECFMP::Mock::FlowMeasure::RouteFilterMock> routeFilter;

        // Dummy expectation, the purpose of this test is to make sure the header compiles properly.
        EXPECT_TRUE(true);
    }
}// namespace ECFMPTest
