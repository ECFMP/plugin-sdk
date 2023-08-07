#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "EuroScopePlugIn.h"
#include "event/ConcreteEvent.h"
#include "event/ConcreteEventParticipant.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteEventFilter.h"
#include "flowmeasure/ConcreteLevelRangeFilter.h"
#include "flowmeasure/ConcreteMultipleLevelFilter.h"
#include "flowmeasure/ConcreteRangeToDestinationFilter.h"
#include "flowmeasure/ConcreteRouteFilter.h"
#include "mock/MockEuroscopeAircraft.h"
#include "mock/MockEuroscopeAircraftFactory.h"

namespace ECFMPTest::FlowMeasure {
    class ConcreteFlowMeasureFiltersTest : public testing::Test
    {
        public:
        ConcreteFlowMeasureFiltersTest()
            : aircraftFactory(std::make_shared<testing::NiceMock<Euroscope::MockEuroscopeAircraftFactory>>()),
              mockAircraft(std::make_shared<testing::NiceMock<Euroscope::MockEuroscopeAircraft>>()),
              filters(
                      std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>{
                              std::make_shared<ECFMP::FlowMeasure::ConcreteAirportFilter>(
                                      std::set<std::string>{"EGLL"}, ECFMP::FlowMeasure::AirportFilterType::Departure
                              )},
                      std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>{
                              std::make_shared<ECFMP::FlowMeasure::ConcreteEventFilter>(
                                      std::make_shared<ECFMP::Event::ConcreteEvent>(
                                              1, "Test", std::chrono::system_clock::now(),
                                              std::chrono::system_clock::now(),
                                              std::make_shared<
                                                      ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                                                      1, "EGTT", "London"
                                              ),
                                              "ABC",
                                              std::vector<std::shared_ptr<ECFMP::Event::EventParticipant>>{
                                                      std::make_shared<ECFMP::Event::ConcreteEventParticipant>(
                                                              1, "EGLL", "EGKK"
                                                      )}
                                      ),
                                      ECFMP::FlowMeasure::EventParticipation::Participating
                              ),
                      },
                      std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>{
                              std::make_shared<ECFMP::FlowMeasure::ConcreteRouteFilter>(std::set<std::string>{"XAMAB"}
                              )},
                      std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>{
                              std::make_shared<ECFMP::FlowMeasure::ConcreteLevelRangeFilter>(
                                      ECFMP::FlowMeasure::LevelRangeFilterType::AtOrBelow, 150
                              )},
                      std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>{
                              std::make_shared<ECFMP::FlowMeasure::ConcreteMultipleLevelFilter>(std::vector<int>{
                                      150, 200})},
                      std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>{
                              std::make_shared<ECFMP::FlowMeasure::ConcreteRangeToDestinationFilter>(100)},
                      aircraftFactory
              )
        {
            ON_CALL(*aircraftFactory, Make(testing::_, testing::_)).WillByDefault(testing::Return(mockAircraft));

            ON_CALL(*mockAircraft, RouteString).WillByDefault(testing::Return("XAMAB"));
            ON_CALL(*mockAircraft, DepartureAirport).WillByDefault(testing::Return("EGLL"));
            ON_CALL(*mockAircraft, CruiseAltitude).WillByDefault(testing::Return(15000));
            ON_CALL(*mockAircraft, RangeToDestination).WillByDefault(testing::Return(99.0));
            ON_CALL(*mockAircraft, Cid).WillByDefault(testing::Return(1));
        }

        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroscopeAircraftFactory>> aircraftFactory;
        std::shared_ptr<testing::NiceMock<Euroscope::MockEuroscopeAircraft>> mockAircraft;
        ECFMP::FlowMeasure::ConcreteFlowMeasureFilters filters;
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
                filters.FirstAirportFilter([](const ECFMP::FlowMeasure::AirportFilter& airportFilter) {
                           return airportFilter.ApplicableToAirport("EGLL");
                       }
                )->AirportStrings()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableAirportFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstAirportFilter([](const ECFMP::FlowMeasure::AirportFilter& airportFilter) {
            return airportFilter.ApplicableToAirport("LPFG");
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableEventFilter)
    {
        EXPECT_EQ(
                ECFMP::FlowMeasure::EventParticipation::Participating,
                filters.FirstEventFilter([](const ECFMP::FlowMeasure::EventFilter& eventFilter) {
                           return eventFilter.Participation() == ECFMP::FlowMeasure::EventParticipation::Participating;
                       }
                )->Participation()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableEventFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstEventFilter([](const ECFMP::FlowMeasure::EventFilter& eventFilter) {
            return eventFilter.Participation() == ECFMP::FlowMeasure::EventParticipation::NotParticipating;
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableLevelFilter)
    {
        EXPECT_EQ(
                ECFMP::FlowMeasure::LevelRangeFilterType::AtOrBelow,
                filters.FirstLevelFilter([](const ECFMP::FlowMeasure::LevelRangeFilter& levelFilter) {
                           return levelFilter.Level() == 150;
                       }
                )->Type()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableLevelFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstLevelFilter([](const ECFMP::FlowMeasure::LevelRangeFilter& levelFilter) {
            return levelFilter.Level() == 160;
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableMultipleLevelFilter)
    {
        EXPECT_EQ(
                std::vector<int>({150, 200}),
                filters.FirstMultipleLevelFilter(
                               [](const ECFMP::FlowMeasure::MultipleLevelFilter& multipleLevelFilter) {
                                   return multipleLevelFilter.Levels() == std::vector<int>({150, 200});
                               }
                )->Levels()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableMultipleLevelFilter)
    {
        EXPECT_EQ(
                nullptr,
                filters.FirstMultipleLevelFilter([](const ECFMP::FlowMeasure::MultipleLevelFilter& multipleLevelFilter
                                                 ) {
                    return multipleLevelFilter.Levels() == std::vector<int>({160, 200});
                })
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableRouteFilter)
    {
        EXPECT_EQ(
                std::set<std::string>({"XAMAB"}),
                filters.FirstRouteFilter([](const ECFMP::FlowMeasure::RouteFilter& routeFilter) {
                           return routeFilter.RouteStrings() == std::set<std::string>({"XAMAB"});
                       }
                )->RouteStrings()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableRouteFilter)
    {
        EXPECT_EQ(nullptr, filters.FirstRouteFilter([](const ECFMP::FlowMeasure::RouteFilter& routeFilter) {
            return routeFilter.RouteStrings() == std::set<std::string>({"VEULE"});
        }));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsApplicableRangeToDestinationFilter)
    {
        EXPECT_EQ(
                100,
                filters.FirstRangeToDestinationFilter(
                               [](const ECFMP::FlowMeasure::RangeToDestinationFilter& rangeToDestinationFilter) {
                                   return rangeToDestinationFilter.Range() == 100;
                               }
                )->Range()
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItReturnsNullptrIfNoApplicableRangeToDestinationFilter)
    {
        EXPECT_EQ(
                nullptr,
                filters.FirstRangeToDestinationFilter(
                        [](const ECFMP::FlowMeasure::RangeToDestinationFilter& rangeToDestinationFilter) {
                            return rangeToDestinationFilter.Range() == 2;
                        }
                )
        );
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesAirportFilters)
    {
        int filterCount = 0;
        filters.ForEachAirportFilter([&filterCount](const ECFMP::FlowMeasure::AirportFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesEventFilters)
    {
        int filterCount = 0;
        filters.ForEachEventFilter([&filterCount](const ECFMP::FlowMeasure::EventFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesRouteFilters)
    {
        int filterCount = 0;
        filters.ForEachRouteFilter([&filterCount](const ECFMP::FlowMeasure::RouteFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesLevelFilters)
    {
        int filterCount = 0;
        filters.ForEachLevelFilter([&filterCount](const ECFMP::FlowMeasure::LevelRangeFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesMultipleLevelFilters)
    {
        int filterCount = 0;
        filters.ForEachMultipleLevelFilter([&filterCount](const ECFMP::FlowMeasure::MultipleLevelFilter& filter) {
            filterCount++;
        });

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItIteratesRangeToDestinationFilters)
    {
        int filterCount = 0;
        filters.ForEachRangeToDestinationFilter(
                [&filterCount](const ECFMP::FlowMeasure::RangeToDestinationFilter& filter) {
                    filterCount++;
                }
        );

        EXPECT_EQ(1, filterCount);
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsApplicableToAircraftIfAllFiltersPass)
    {
        EXPECT_TRUE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsNotApplicableToAircraftIfAirportFiltersFail)
    {
        ON_CALL(*mockAircraft, DepartureAirport).WillByDefault(testing::Return("EGKK"));
        EXPECT_FALSE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsNotApplicableToAircraftIfEventFiltersFail)
    {
        ON_CALL(*mockAircraft, Cid).WillByDefault(testing::Return(2));
        EXPECT_FALSE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsNotApplicableToAircraftIfLevelRangeFiltersFail)
    {
        ON_CALL(*mockAircraft, CruiseAltitude).WillByDefault(testing::Return(20000));
        EXPECT_FALSE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsNotApplicableToAircraftIfMultipleLevelFiltersFail)
    {
        ON_CALL(*mockAircraft, CruiseAltitude).WillByDefault(testing::Return(14000));
        EXPECT_FALSE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsNotApplicableToAircraftIfRouteFiltersFail)
    {
        ON_CALL(*mockAircraft, RouteString).WillByDefault(testing::Return("VEULE"));
        EXPECT_FALSE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }

    TEST_F(ConcreteFlowMeasureFiltersTest, ItsNotApplicableToAircraftIfRangeToDestinationFiltersFail)
    {
        ON_CALL(*mockAircraft, RangeToDestination).WillByDefault(testing::Return(101.0));
        EXPECT_FALSE(filters.ApplicableToAircraft(EuroScopePlugIn::CFlightPlan(), EuroScopePlugIn::CRadarTarget()));
    }
}// namespace ECFMPTest::FlowMeasure
