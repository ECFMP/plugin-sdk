#include "flowmeasure/ConcreteFlowMeasure.h"
#include "event/ConcreteEvent.h"
#include "flightinformationregion/ConcreteFlightInformationRegion.h"
#include "flowmeasure/ConcreteAirportFilter.h"
#include "flowmeasure/ConcreteFlowMeasureFilters.h"
#include "flowmeasure/ConcreteMeasure.h"

namespace FlowSdkTest::FlowMeasure {
    class ConcreteFlowMeasureTest : public testing::Test
    {
        public:
        ConcreteFlowMeasureTest()
            : startTime(std::chrono::system_clock::now() - std::chrono::minutes(15)),
              endTime(std::chrono::system_clock::now() + std::chrono::minutes(15)),
              withdrawnTime(std::chrono::system_clock::now()),
              event(std::make_shared<FlowSdk::Event::ConcreteEvent>(
                      1, "Test", std::chrono::system_clock::now(), std::chrono::system_clock::now(),
                      std::make_shared<FlowSdk::FlightInformationRegion::ConcreteFlightInformationRegion>(
                              1, "EGTT", "London"
                      ),
                      "ABC"
              )),
              measure1(
                      1, event, "EGTT01A", "Reason", startTime, endTime, withdrawnTime,
                      FlowSdk::FlowMeasure::MeasureStatus::Active, {},
                      std::make_unique<FlowSdk::FlowMeasure::ConcreteMeasure>(
                              FlowSdk::FlowMeasure::MeasureType::Prohibit
                      ),
                      std::make_unique<FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters>(
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::AirportFilter>>{
                                      std::make_shared<FlowSdk::FlowMeasure::ConcreteAirportFilter>(
                                              std::set<std::string>{"EGLL"},
                                              FlowSdk::FlowMeasure::AirportFilterType::Departure
                                      )},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::EventFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::RouteFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::LevelFilter>>{}
                      )
              ),
              measureWithNoEvent(
                      2, nullptr, "EGTT01A", "Reason", startTime, endTime, withdrawnTime,
                      FlowSdk::FlowMeasure::MeasureStatus::Active, {},
                      std::make_unique<FlowSdk::FlowMeasure::ConcreteMeasure>(
                              FlowSdk::FlowMeasure::MeasureType::Prohibit
                      ),
                      std::make_unique<FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters>(
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::AirportFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::EventFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::RouteFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::LevelFilter>>{}
                      )
              ),
              withdrawnMeasure(
                      3, nullptr, "EGTT01A", "Reason", startTime, endTime, withdrawnTime,
                      FlowSdk::FlowMeasure::MeasureStatus::Withdrawn, {},
                      std::make_unique<FlowSdk::FlowMeasure::ConcreteMeasure>(
                              FlowSdk::FlowMeasure::MeasureType::Prohibit
                      ),
                      std::make_unique<FlowSdk::FlowMeasure::ConcreteFlowMeasureFilters>(
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::AirportFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::EventFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::RouteFilter>>{},
                              std::list<std::shared_ptr<FlowSdk::FlowMeasure::LevelFilter>>{}
                      )
              )
        {}

        std::chrono::system_clock::time_point startTime;
        std::chrono::system_clock::time_point endTime;
        std::chrono::system_clock::time_point withdrawnTime;
        std::shared_ptr<FlowSdk::Event::ConcreteEvent> event;
        FlowSdk::FlowMeasure::ConcreteFlowMeasure measure1;
        FlowSdk::FlowMeasure::ConcreteFlowMeasure measureWithNoEvent;
        FlowSdk::FlowMeasure::ConcreteFlowMeasure withdrawnMeasure;
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
        EXPECT_THROW(static_cast<void>(measure1.WithdrawnAt()), FlowSdk::FlowMeasure::FlowMeasureNotWithdrawnException);
        EXPECT_THROW(
                static_cast<void>(measureWithNoEvent.WithdrawnAt()),
                FlowSdk::FlowMeasure::FlowMeasureNotWithdrawnException
        );
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAStatus)
    {
        EXPECT_EQ(FlowSdk::FlowMeasure::MeasureStatus::Active, measure1.Status());
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsAStatus)
    {
        EXPECT_TRUE(measure1.HasStatus(FlowSdk::FlowMeasure::MeasureStatus::Active));
    }

    TEST_F(ConcreteFlowMeasureTest, ItIsNotAStatus)
    {
        EXPECT_FALSE(measure1.HasStatus(FlowSdk::FlowMeasure::MeasureStatus::Withdrawn));
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasNotifiedFlightInformationRegions)
    {
        EXPECT_EQ(
                std::vector<std::shared_ptr<const FlowSdk::FlightInformationRegion::FlightInformationRegion>>(),
                measure1.NotifiedFlightInformationRegions()
        );
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasAMeasure)
    {
        EXPECT_EQ(FlowSdk::FlowMeasure::MeasureType::Prohibit, measure1.Measure().Type());
    }

    TEST_F(ConcreteFlowMeasureTest, ItHasFilters)
    {
        EXPECT_TRUE(measure1.Filters().ApplicableToAirport("EGLL"));
        EXPECT_FALSE(measure1.Filters().ApplicableToAirport("EGKK"));
    }
}// namespace FlowSdkTest::FlowMeasure
