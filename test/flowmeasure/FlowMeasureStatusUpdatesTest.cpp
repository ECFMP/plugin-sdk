#include <utility>

#include "../../include/mock/FlowMeasureMock.h"
#include "ECFMP/SdkEvents.h"
#include "ECFMP/eventbus/EventListener.h"
#include "ECFMP/flowmeasure/CanonicalFlowMeasureInfo.h"
#include "ECFMP/flowmeasure/FlowMeasure.h"
#include "api/InternalStringIdentifierApiElementCollection.h"
#include "eventbus/InternalEventBusFactory.h"
#include "flowmeasure/FlowMeasureStatusUpdates.h"
#include "log/NullLogger.h"
#include "gmock/gmock.h"

namespace ECFMPTest::FlowMeasure {

    template<typename EventType>
    class FlowMeasureStatusUpdatesEventListener : public ECFMP::EventBus::EventListener<EventType>
    {
        public:
        FlowMeasureStatusUpdatesEventListener(std::shared_ptr<ECFMP::FlowMeasure::FlowMeasure> expectedMeasure)
            : expectedMeasure(std::move(expectedMeasure))
        {}

        void OnEvent(const EventType& event) override
        {
            callCount++;
            EXPECT_EQ(event.flowMeasure, expectedMeasure);
        }

        void AssertCalledOnce()
        {
            EXPECT_EQ(callCount, 1);
        }

        void AssertNotCalled()
        {
            EXPECT_EQ(callCount, 0);
        }

        private:
        int callCount = 0;

        std::shared_ptr<ECFMP::FlowMeasure::FlowMeasure> expectedMeasure;
    };

    class FlowMeasureReissuedEventListener
        : public ECFMP::EventBus::EventListener<ECFMP::Plugin::FlowMeasureReissuedEvent>
    {
        public:
        FlowMeasureReissuedEventListener(
                std::shared_ptr<ECFMP::FlowMeasure::FlowMeasure> expectedOriginal,
                std::shared_ptr<ECFMP::FlowMeasure::FlowMeasure> expectedReissued
        )
            : expectedOriginal(std::move(expectedOriginal)), expectedReissued(std::move(expectedReissued))
        {}

        void OnEvent(const ECFMP::Plugin::FlowMeasureReissuedEvent& event) override
        {
            EXPECT_EQ(event.original, expectedOriginal);
            EXPECT_EQ(event.reissued, expectedReissued);
            callCount++;
        }

        void AssertCalledOnce()
        {
            ASSERT_EQ(callCount, 1);
        }

        void AssertNotCalled()
        {
            ASSERT_EQ(callCount, 0);
        }

        private:
        int callCount = 0;

        std::shared_ptr<ECFMP::FlowMeasure::FlowMeasure> expectedOriginal;
        std::shared_ptr<ECFMP::FlowMeasure::FlowMeasure> expectedReissued;
    };

    class FlowMeasureStatusUpdatesTest : public ::testing::Test
    {
        public:
        FlowMeasureStatusUpdatesTest()
            : eventBus(ECFMP::EventBus::MakeEventBus()), logger(std::make_shared<ECFMP::Log::NullLogger>()),
              flowMeasureStatusUpdates(std::make_shared<ECFMP::FlowMeasure::FlowMeasureStatusUpdates>(eventBus, logger))
        {}

        [[nodiscard]] static auto
        GetMeasureMock(const std::string& identifier, const ECFMP::FlowMeasure::MeasureStatus status)
                -> std::shared_ptr<ECFMP::Mock::FlowMeasure::FlowMeasureMock>
        {
            auto measure = std::make_shared<ECFMP::Mock::FlowMeasure::FlowMeasureMock>();
            auto canonicalInformation = std::make_shared<ECFMP::FlowMeasure::CanonicalFlowMeasureInfo>(identifier);

            ON_CALL(*measure, CanonicalInformation)
                    .WillByDefault(testing::Invoke(
                            [canonicalInformation]() -> const ECFMP::FlowMeasure::CanonicalFlowMeasureInfo& {
                                return *canonicalInformation;
                            }
                    ));
            ON_CALL(*measure, Status).WillByDefault(testing::Return(status));
            ON_CALL(*measure, Identifier).WillByDefault(testing::Invoke([identifier]() -> const std::string& {
                return identifier;
            }));

            return measure;
        }

        protected:
        std::shared_ptr<ECFMP::EventBus::InternalEventBus> eventBus;
        std::shared_ptr<ECFMP::Log::Logger> logger;
        std::shared_ptr<ECFMP::FlowMeasure::FlowMeasureStatusUpdates> flowMeasureStatusUpdates;
    };

    TEST_F(FlowMeasureStatusUpdatesTest, ItBroadcastsNotifiedEventFirstTimeCanonicalMeasureSeen)
    {
        // Set up the measure and collection
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Notified);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure1
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure1
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure1
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure1
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertCalledOnce();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertNotCalled();
        listenerExpired->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItBroadcastsActivatedEventFirstTimeCanonicalMeasureSeen)
    {
        // Set up the measure and collection
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Active);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure1
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure1
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure1
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure1
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertCalledOnce();
        listenerWithdrawn->AssertNotCalled();
        listenerExpired->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItBroadcastsWithdrawnEventFirstTimeCanonicalMeasureSeen)
    {
        // Set up the measure and collection
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Withdrawn);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure1
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure1
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure1
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure1
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertCalledOnce();
        listenerExpired->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItBroadcastsExpiredEventFirstTimeCanonicalMeasureSeen)
    {
        // Set up the measure and collection
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Expired);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure1
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure1
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure1
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure1
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertNotCalled();
        listenerExpired->AssertCalledOnce();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItBroadcastsReissuedEventOnCanonicalUpodate)
    {
        // Set up the measure and collection
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Expired);
        auto measure2 = GetMeasureMock("EGTT01A-1", ECFMP::FlowMeasure::MeasureStatus::Expired);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);

        // Set up event listeners
        auto listenerReissued = std::make_shared<FlowMeasureReissuedEventListener>(measure1, measure2);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureReissuedEvent>(listenerReissued);

        // Run event and check no first-time broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerReissued->AssertNotCalled();

        // Add the second measure and check the reissued event is broadcast
        flowMeasures->Add(measure2);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerReissued->AssertCalledOnce();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItDoesntBroadcastReissuedEventOnCanonicalUpodate)
    {
        // Set up the measure and collection
        auto measure1 = GetMeasureMock("EGTT01A-1", ECFMP::FlowMeasure::MeasureStatus::Expired);
        auto measure2 = GetMeasureMock("EGTT01A0", ECFMP::FlowMeasure::MeasureStatus::Expired);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);

        // Set up event listeners
        auto listenerReissued = std::make_shared<FlowMeasureReissuedEventListener>(measure1, measure2);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureReissuedEvent>(listenerReissued);

        // Run event and check no first-time broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerReissued->AssertNotCalled();

        // Add the second measure and check the reissued event is still not broadcast
        flowMeasures->Add(measure2);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerReissued->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItTransitionsAMeasureFromNotifiedToActivated)
    {
        // Set up the measure and collection, then call the event to emplant the measure
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Notified);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        // Create the second measure
        auto measure2 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Active);
        flowMeasures->Add(measure2);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure2
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure2
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure2
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure2
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertCalledOnce();
        listenerWithdrawn->AssertNotCalled();
        listenerExpired->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItTransitionsAMeasureFromNotifiedToExpired)
    {
        // Set up the measure and collection, then call the event to emplant the measure
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Notified);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        // Create the second measure
        auto measure2 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Expired);
        flowMeasures->Add(measure2);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure2
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure2
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure2
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure2
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertNotCalled();
        listenerExpired->AssertCalledOnce();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItTransitionsAMeasureFromNotifiedToAWithdrawn)
    {
        // Set up the measure and collection, then call the event to emplant the measure
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Notified);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        // Create the second measure
        auto measure2 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Withdrawn);
        flowMeasures->Add(measure2);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure2
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure2
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure2
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure2
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertCalledOnce();
        listenerExpired->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItTransitionsAMeasureFromActiveToExpired)
    {
        // Set up the measure and collection, then call the event to emplant the measure
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Active);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        // Create the second measure
        auto measure2 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Expired);
        flowMeasures->Add(measure2);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure2
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure2
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure2
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure2
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertNotCalled();
        listenerExpired->AssertCalledOnce();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItTransitionsAMeasureFromActiveToWithdrawn)
    {
        // Set up the measure and collection, then call the event to emplant the measure
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Active);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        // Create the second measure
        auto measure2 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Withdrawn);
        flowMeasures->Add(measure2);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure2
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure2
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure2
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure2
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertCalledOnce();
        listenerExpired->AssertNotCalled();
    }

    TEST_F(FlowMeasureStatusUpdatesTest, ItTransitionsAMeasureThroughMultipleStatuses)
    {
        // Set up the measure and collection, then call the event to emplant the measure
        auto measure1 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Notified);
        std::shared_ptr<ECFMP::Api::InternalFlowMeasureCollection> flowMeasures =
                std::make_shared<ECFMP::Api::InternalFlowMeasureCollection>();
        flowMeasures->Add(measure1);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        // Create the second and third measure
        auto measure2 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Active);
        flowMeasures->Add(measure2);
        flowMeasureStatusUpdates->OnEvent({flowMeasures});

        auto measure3 = GetMeasureMock("EGTT01A", ECFMP::FlowMeasure::MeasureStatus::Withdrawn);
        flowMeasures->Add(measure3);

        // Set up event listeners
        auto listenerNotified =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin ::FlowMeasureNotifiedEvent>>(
                        measure3
                );
        auto listenerActivated =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureActivatedEvent>>(
                        measure3
                );
        auto listenerWithdrawn =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureWithdrawnEvent>>(
                        measure3
                );
        auto listenerExpired =
                std::make_shared<FlowMeasureStatusUpdatesEventListener<ECFMP::Plugin::FlowMeasureExpiredEvent>>(measure3
                );

        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureNotifiedEvent>(listenerNotified);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureActivatedEvent>(listenerActivated);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureWithdrawnEvent>(listenerWithdrawn);
        eventBus->SubscribeSync<ECFMP::Plugin::FlowMeasureExpiredEvent>(listenerExpired);

        // Run event and check the events broadcast
        flowMeasureStatusUpdates->OnEvent({flowMeasures});
        listenerNotified->AssertNotCalled();
        listenerActivated->AssertNotCalled();
        listenerWithdrawn->AssertCalledOnce();
        listenerExpired->AssertNotCalled();
    }
}// namespace ECFMPTest::FlowMeasure
