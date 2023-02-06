#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteEventFilter.h"
#include "flowmeasure/ConcreteLevelFilter.h"
#include "flowmeasure/ConcreteRouteFilter.h"

namespace FlowSdkTest::FlowMeasure {
    class ConcreteFlowMeasureFiltersTest : public testing::Test
    {
        public:
        ConcreteFlowMeasureFiltersTest()
            : filters(
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::AirportFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteAirportFilter>(
                                    std::set<std::string>{"EGLL"})},
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::EventFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteEventFilter>(
                                    std::make_shared<FlowSdk::Event::ConcreteEvent>(
                                            1, "Test", std::chrono::system_clock::now(),
                                            std::chrono::system_clock::now(),
                                            std::make_shared<
                                                    FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                                                    1, "EGTT", "London"),
                                            "ABC"),
                                    true),
                    },
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::RouteFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteRouteFilter>(
                                    std::set<std::string>{"XAMAB"})},
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::LevelFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteLevelFilter>(
                                    FlowSdk::FlowMeasure::LevelFilterType::At, 150)},
                    std::list<std::shared_ptr<FlowSdk::FlightInformationRegion::FlightInformationRegion>>{
                            std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                                    1, "EGTT", "London")})
        {}

        FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters filters;
    };

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsApplicableToAirport)
    {
        EXPECT_TRUE(filters.ApplicableToAirport("EGLL"));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsNotApplicableToAirport)
    {
        EXPECT_FALSE(filters.ApplicableToAirport("EGKK"));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsApplicableToFlightInformationRegionById)
    {
        EXPECT_TRUE(filters.ApplicableToFlightInformationRegion(1));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsNotApplicableToFlightInformationRegionById)
    {
        EXPECT_FALSE(filters.ApplicableToFlightInformationRegion(2));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsApplicableToFlightInformationRegionByIdentifier)
    {
        EXPECT_TRUE(filters.ApplicableToFlightInformationRegion("EGTT"));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsNotApplicableToFlightInformationRegionByIdentifier)
    {
        EXPECT_FALSE(filters.ApplicableToFlightInformationRegion("EGLL"));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsApplicableToFlightInformationRegionByInstance)
    {
        FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion region(1, "EGTT", "London");
        EXPECT_TRUE(filters.ApplicableToFlightInformationRegion(region));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIsNotApplicableToFlightInformationRegionByInstance)
    {
        FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion region(2, "EGTT", "London");
        EXPECT_FALSE(filters.ApplicableToFlightInformationRegion(region));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableAirportFilter)
    {
        EXPECT_EQ(std::set<std::string>({"EGLL"}),
                  filters.FirstAirportFilter([](const FlowSdk::FlowMeasure::AirportFilter& airportFilter) {
                             return airportFilter.ApplicableToAirport("EGLL");
                         })
                          ->AirportStrings());
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableAirportFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstAirportFilter([](const FlowSdk::FlowMeasure::AirportFilter& airportFilter) {
            return airportFilter.ApplicableToAirport("LPFG");
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableEventFilter)
    {
        EXPECT_EQ(true,
                  filters.FirstEventFilter([](const FlowSdk::FlowMeasure::EventFilter& eventFilter) {
                             return eventFilter.ParticipatingIn();
                         })
                          ->ParticipatingIn());
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableEventFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstEventFilter([](const FlowSdk::FlowMeasure::EventFilter& eventFilter) {
            return !eventFilter.ParticipatingIn();
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableLevelFilter)
    {
        EXPECT_EQ(FlowSdk::FlowMeasure::LevelFilterType::At,
                  filters.FirstLevelFilter([](const FlowSdk::FlowMeasure::LevelFilter& levelFilter) {
                             return levelFilter.Level() == 150;
                         })
                          ->Type());
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableLevelFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstLevelFilter([](const FlowSdk::FlowMeasure::LevelFilter& levelFilter) {
            return levelFilter.Level() == 160;
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableRouteFilter)
    {
        EXPECT_EQ(std::set<std::string>({"XAMAB"}),
                  filters.FirstRouteFilter([](const FlowSdk::FlowMeasure::RouteFilter& routeFilter) {
                             return routeFilter.RouteStrings() == std::set<std::string>({"XAMAB"});
                         })
                          ->RouteStrings());
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableRouteFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstRouteFilter([](const FlowSdk::FlowMeasure::RouteFilter& routeFilter) {
            return routeFilter.RouteStrings() == std::set<std::string>({"VEULE"});
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesAirportFilters)
    {
        int filterCount = 0;
        filters.ForEachAirportFilter([&filterCount](const FlowSdk::FlowMeasure::AirportFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesEventFilters)
    {
        int filterCount = 0;
        filters.ForEachEventFilter([&filterCount](const FlowSdk::FlowMeasure::EventFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesRouteFilters)
    {
        int filterCount = 0;
        filters.ForEachRouteFilter([&filterCount](const FlowSdk::FlowMeasure::RouteFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesLevelFilters)
    {
        int filterCount = 0;
        filters.ForEachLevelFilter([&filterCount](const FlowSdk::FlowMeasure::LevelFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }
}// namespace FlowSdkTest::FlowMeasure
