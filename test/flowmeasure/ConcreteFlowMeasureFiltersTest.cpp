#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteEventFilter.h"
#include "flowmeasure/ConcreteLevelRangeFilter.h"
#include "flowmeasure/ConcreteMultipleLevelFilter.h"
#include "flowmeasure/ConcreteRouteFilter.h"

namespace FlowSdkTest::FlowMeasure {
    class ConcreteFlowMeasureFiltersTest : public testing::Test
    {
        public:
        ConcreteFlowMeasureFiltersTest()
            : filters(
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::AirportFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteAirportFilter>(
                                    std::set<std::string>{"EGLL"}, FlowSdk::FlowMeasure::AirportFilterType::Departure
                            )},
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::EventFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteEventFilter>(
                                    std::make_shared<FlowSdk::Event::ConcreteEvent>(
                                            1, "Test", std::chrono::system_clock::now(),
                                            std::chrono::system_clock::now(),
                                            std::make_shared<
                                                    FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                                                    1, "EGTT", "London"
                                            ),
                                            "ABC"
                                    ),
                                    FlowSdk::FlowMeasure::EventParticipation::Participating
                            ),
                    },
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::RouteFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteRouteFilter>(std::set<std::string>{"XAMAB"}
                            )},
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::LevelRangeFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteLevelRangeFilter>(
                                    FlowSdk::FlowMeasure::LevelRangeFilterType::AtOrBelow, 150
                            )},
                    std::list<std::shared_ptr<FlowSdk::FlowMeasure::MultipleLevelFilter>>{
                            std::make_shared<FlowSdk::FlowMeasure::ConcreteMultipleLevelFilter>(std::vector<int>{
                                    150, 200})}
            )
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

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableAirportFilter)
    {
        EXPECT_EQ(
                std::set<std::string>({"EGLL"}),
                filters.FirstAirportFilter([](const FlowSdk::FlowMeasure::AirportFilter& airportFilter) {
                           return airportFilter.ApplicableToAirport("EGLL");
                       }
                )->AirportStrings()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableAirportFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstAirportFilter([](const FlowSdk::FlowMeasure::AirportFilter& airportFilter) {
            return airportFilter.ApplicableToAirport("LPFG");
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableEventFilter)
    {
        EXPECT_EQ(
                FlowSdk::FlowMeasure::EventParticipation::Participating,
                filters.FirstEventFilter([](const FlowSdk::FlowMeasure::EventFilter& eventFilter) {
                           return eventFilter.Participation()
                                   == FlowSdk::FlowMeasure::EventParticipation::Participating;
                       }
                )->Participation()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableEventFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstEventFilter([](const FlowSdk::FlowMeasure::EventFilter& eventFilter) {
            return eventFilter.Participation() == FlowSdk::FlowMeasure::EventParticipation::NotParticipating;
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableLevelFilter)
    {
        EXPECT_EQ(
                FlowSdk::FlowMeasure::LevelRangeFilterType::AtOrBelow,
                filters.FirstLevelFilter([](const FlowSdk::FlowMeasure::LevelRangeFilter& levelFilter) {
                           return levelFilter.Level() == 150;
                       }
                )->Type()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableLevelFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstLevelFilter([](const FlowSdk::FlowMeasure::LevelRangeFilter& levelFilter) {
            return levelFilter.Level() == 160;
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableMultipleLevelFilter)
    {
        EXPECT_EQ(
                std::vector<int>({150, 200}),
                filters.FirstMultipleLevelFilter(
                               [](const FlowSdk::FlowMeasure::MultipleLevelFilter& multipleLevelFilter) {
                                   return multipleLevelFilter.Levels() == std::vector<int>({150, 200});
                               }
                )->Levels()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableMultipleLevelFilter)
    {
        EXPECT_EQ(
                nullptr,
                filters.FirstMultipleLevelFilter([](const FlowSdk::FlowMeasure::MultipleLevelFilter& multipleLevelFilter
                                                 ) {
                    return multipleLevelFilter.Levels() == std::vector<int>({160, 200});
                })
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableRouteFilter)
    {
        EXPECT_EQ(
                std::set<std::string>({"XAMAB"}),
                filters.FirstRouteFilter([](const FlowSdk::FlowMeasure::RouteFilter& routeFilter) {
                           return routeFilter.RouteStrings() == std::set<std::string>({"XAMAB"});
                       }
                )->RouteStrings()
        );
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
        filters.ForEachLevelFilter([&filterCount](const FlowSdk::FlowMeasure::LevelRangeFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesMultipleLevelFilters)
    {
        int filterCount = 0;
        filters.ForEachMultipleLevelFilter([&filterCount](const FlowSdk::FlowMeasure::MultipleLevelFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }
}// namespace FlowSdkTest::FlowMeasure
