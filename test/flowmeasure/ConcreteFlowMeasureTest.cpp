#include "flowmeasure/ConcreteFlowMeasure.h"
#include "ECFMP/flightinformationregion/FlightInformationRegion.h"
#include "ECFMP/flowmeasure/MultipleLevelFilter.h"
#include "ECFMP/flowmeasure/RangeToDestinationFilter.h"
#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "flowmeasure/ConcreteMeasure.h"

namespace ECFMPTest::FlowMeasure {
    class ConcreteFlowMeasureTest : public testing::Test
    {
        public:
        ConcreteFlowMeasureTest()
            : fir1(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                    1, "EGTT", "London"
            )),
              fir2(std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                      2, "EGPX", "Scottish"
              )),
              startTime(std::chrono::system_clock::now() - std::chrono::minutes(15)),
              endTime(std::chrono::system_clock::now() + std::chrono::minutes(15)),
              withdrawnTime(std::chrono::system_clock::now()),
              event(std::make_shared<ECFMP::Event::ConcreteEvent>(
                      1, "Test", std::chrono::system_clock::now(), std::chrono::system_clock::now(),
                      std::make_shared<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion>(
                              1, "EGTT", "London"
                      ),
                      "ABC"
              )),
              measure1(
                      1, event, "EGTT01A", "Reason", startTime, endTime, withdrawnTime,
                      ECFMP::FlowMeasure::MeasureStatus::Active, {fir1},
                      std::make_unique<ECFMP::FlowMeasure::ConcreteMeasure>(ECFMP::FlowMeasure::MeasureType::Prohibit),
                      std::make_unique<ECFMP::FlowMeasure::ConcreteFlowMeasureFilters>(
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>{
                                      std::make_shared<ECFMP::FlowMeasure::ConcreteAirportFilter>(
                                              std::set<std::string>{"EGLL"},
                                              ECFMP::FlowMeasure::AirportFilterType::Departure
                                      )},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>{}
                      )
              ),
              measureWithNoEvent(
                      2, nullptr, "EGTT01A", "Reason", startTime, endTime, withdrawnTime,
                      ECFMP::FlowMeasure::MeasureStatus::Active, {},
                      std::make_unique<ECFMP::FlowMeasure::ConcreteMeasure>(ECFMP::FlowMeasure::MeasureType::Prohibit),
                      std::make_unique<ECFMP::FlowMeasure::ConcreteFlowMeasureFilters>(
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>{}
                      )
              ),
              withdrawnMeasure(
                      3, nullptr, "EGTT01A", "Reason", startTime, endTime, withdrawnTime,
                      ECFMP::FlowMeasure::MeasureStatus::Withdrawn, {},
                      std::make_unique<ECFMP::FlowMeasure::ConcreteMeasure>(ECFMP::FlowMeasure::MeasureType::Prohibit),
                      std::make_unique<ECFMP::FlowMeasure::ConcreteFlowMeasureFilters>(
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::AirportFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::EventFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::RouteFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::LevelRangeFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::MultipleLevelFilter>>{},
                              std::list<std::shared_ptr<ECFMP::FlowMeasure::RangeToDestinationFilter>>{}
                      )
              )
        {}

        std::shared_ptr<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion> fir1;
        std::shared_ptr<ECFMP::FlightInformationRegion::ConcreteFlightInformationRegion> fir2;
        std::chrono::system_clock::time_point startTime;
        std::chrono::system_clock::time_point endTime;
        std::chrono::system_clock::time_point withdrawnTime;
        std::shared_ptr<ECFMP::Event::ConcreteEvent> event;
        ECFMP::FlowMeasure::ConcreteFlowMeasure measure1;
        ECFMP::FlowMeasure::ConcreteFlowMeasure measureWithNoEvent;
        ECFMP::FlowMeasure::ConcreteFlowMeasure withdrawnMeasure;
    };

    TEST_F(ConcreteFlowMeasureTest, ItHasAnId)
    {
        EXPECT_EQ(1, measure1.Id());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAnEvent)
    {
        EXPECT_EQ(event, measure1.Event());
    }

    TEST_F(ConcreteFlowMeasureTest, ItReturnsNullptrIfNoEvent)
    {
        EXPECT_EQ(nullptr, measureWithNoEvent.Event());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAnIdentifier)
    {
        EXPECT_EQ("EGTT01A", measure1.Identifier());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAReason)
    {
        EXPECT_EQ("Reason", measure1.Reason());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAStartTime)
    {
        EXPECT_EQ(startTime, measure1.StartTime());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAEndTime)
    {
        EXPECT_EQ(endTime, measure1.EndTime());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAWithdrawnTime)
    {
        EXPECT_EQ(withdrawnTime, withdrawnMeasure.WithdrawnAt());
    }

    TEST_F(ConcreteFlowMeasureTest, ItThrowsAnExceptionIfNotWithdrawn)
    {
        EXPECT_THROW(static_cast<void>(measure1.WithdrawnAt()), ECFMP::FlowMeasure::FlowMeasureNotWithdrawnException);
        EXPECT_THROW(
                static_cast<void>(measureWithNoEvent.WithdrawnAt()),
                ECFMP::FlowMeasure::FlowMeasureNotWithdrawnException
        );
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAStatus)
    {
        EXPECT_EQ(ECFMP::FlowMeasure::MeasureStatus::Active, measure1.Status());
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsAStatus)
    {
        EXPECT_TRUE(measure1.HasStatus(ECFMP::FlowMeasure::MeasureStatus::Active));
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsNotAStatus)
    {
        EXPECT_FALSE(measure1.HasStatus(ECFMP::FlowMeasure::MeasureStatus::Withdrawn));
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasNotifiedFlightInformationRegions)
    {
        EXPECT_EQ(
                std::vector<std::shared_ptr<const ECFMP::FlightInformationRegion::FlightInformationRegion>>({fir1}),
                measure1.NotifiedFlightInformationRegions()
        );
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAMeasure)
    {
        EXPECT_EQ(ECFMP::FlowMeasure::MeasureType::Prohibit, measure1.Measure().Type());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasFilters)
    {
        EXPECT_TRUE(measure1.Filters().ApplicableToAirport("EGLL"));
        EXPECT_FALSE(measure1.Filters().ApplicableToAirport("EGKK"));
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsApplicableToFlightInformationRegionByModel)
    {
        EXPECT_TRUE(measure1.IsApplicableToFlightInformationRegion(*fir1));
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsNotApplicableToFlightInformationRegionByModel)
    {
        EXPECT_FALSE(measure1.IsApplicableToFlightInformationRegion(*fir2));
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsApplicableToFlightInformationRegionByCode)
    {
        EXPECT_TRUE(measure1.IsApplicableToFlightInformationRegion("EGTT"));
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsNotApplicableToFlightInformationRegionByCode)
    {
        EXPECT_FALSE(measure1.IsApplicableToFlightInformationRegion("EGPX"));
    }
}// namespace ECFMPTest::FlowMeasure
