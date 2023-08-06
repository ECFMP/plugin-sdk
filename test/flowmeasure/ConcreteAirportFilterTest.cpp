#include "flowmeasure/ConcreteAirportFilter.h"
#include "mock/MockEuroscopeAircraft.h"

namespace ECFMPTest::FlowMeasure {

    using AirportFilterCheck = struct AirportFilterCheck {
        std::string airport;

        bool expected;
    };

    class ConcreteAirportFilterTest : public testing::Test
    {
        public:
        ConcreteAirportFilterTest()
            : airportFilter(
                    std::set<std::string>{"EGKK", "LF", "EDD", "K"}, ECFMP::FlowMeasure::AirportFilterType::Destination
            )
        {}

        ECFMP::FlowMeasure::ConcreteAirportFilter airportFilter;
    };

    TEST_F(ConcreteAirportFilterTest, ItReturnsAirportStrings)
    {
        EXPECT_EQ(std::set<std::string>({"EGKK", "LF", "EDD", "K"}), airportFilter.AirportStrings());
    }

    TEST_F(ConcreteAirportFilterTest, ItReturnsAirportFilterType)
    {
        EXPECT_EQ(ECFMP::FlowMeasure::AirportFilterType::Destination, airportFilter.Type());
    }

    class ConcreteAirportFilterApplicabilityTest : public testing::TestWithParam<AirportFilterCheck>
    {
        public:
        [[nodiscard]] static auto GetFilter(ECFMP::FlowMeasure::AirportFilterType type)
                -> ECFMP::FlowMeasure::ConcreteAirportFilter
        {
            return ECFMP::FlowMeasure::ConcreteAirportFilter(std::set<std::string>{"EGKK", "LF", "EDD", "K"}, type);
        }
    };

    TEST_P(ConcreteAirportFilterApplicabilityTest, ItChecksAirportApplicability)
    {
        EXPECT_EQ(
                GetParam().expected,
                GetFilter(ECFMP::FlowMeasure::AirportFilterType::Destination).ApplicableToAirport(GetParam().airport)
        );
    }

    TEST_P(ConcreteAirportFilterApplicabilityTest, ItChecksDepartureAirportApplicabilityForAircraft)
    {
        const auto aircraft = testing::NiceMock<Euroscope::MockEuroscopeAircraft>();
        EXPECT_CALL(aircraft, DepartureAirport).WillRepeatedly(testing::Return(GetParam().airport));

        EXPECT_EQ(
                GetParam().expected,
                GetFilter(ECFMP::FlowMeasure::AirportFilterType::Departure).ApplicableToAirport(GetParam().airport)
        );
    }

    TEST_P(ConcreteAirportFilterApplicabilityTest, ItChecksDestinationAirportApplicabilityForAircraft)
    {
        const auto aircraft = testing::NiceMock<Euroscope::MockEuroscopeAircraft>();
        EXPECT_CALL(aircraft, DepartureAirport).WillRepeatedly(testing::Return(GetParam().airport));

        EXPECT_EQ(
                GetParam().expected,
                GetFilter(ECFMP::FlowMeasure::AirportFilterType::Destination).ApplicableToAirport(GetParam().airport)
        );
    }

    INSTANTIATE_TEST_SUITE_P(
            ConcreteAirportFilterApplicabilityTestCases, ConcreteAirportFilterApplicabilityTest,
            testing::Values(
                    AirportFilterCheck{"EGKK", true}, AirportFilterCheck{"LFPG", true}, AirportFilterCheck{"LFP", true},
                    AirportFilterCheck{"LF", true}, AirportFilterCheck{"EDDM", true}, AirportFilterCheck{"EDDF", true},
                    AirportFilterCheck{"K", true}, AirportFilterCheck{"KJ", true}, AirportFilterCheck{"KJF", true},
                    AirportFilterCheck{"KJFK", true}, AirportFilterCheck{"EGLL", false},
                    AirportFilterCheck{"EGG", false}, AirportFilterCheck{"EG", false}, AirportFilterCheck{"E", false},
                    AirportFilterCheck{"LRPG", false}, AirportFilterCheck{"L", false},
                    AirportFilterCheck{"EDGM", false}, AirportFilterCheck{"X", false}
            ),
            [](const ::testing::TestParamInfo<ConcreteAirportFilterApplicabilityTest::ParamType>& info) {
                return info.param.airport;
            }
    );
}// namespace ECFMPTest::FlowMeasure
