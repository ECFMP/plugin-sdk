#include "flow-sdk/SdkFactory.h"
#include "ConcreteEventListeners.h"
#include "ConcreteSdk.h"
#include "api/ApiDataDownloader.h"
#include "api/ApiDataScheduler.h"
#include "api/ConcreteApiElementCollection.h"
#include "api/ConcreteStringIdentifiedApiElementCollection.h"
#include "api/EventDataParser.h"
#include "api/FlightInformationRegionDataParser.h"
#include "api/FlowMeasureDataParser.h"
#include "api/FlowMeasureFilterParser.h"
#include "api/FlowMeasureMeasureParser.h"
#include "eventbus/InternalEventBus.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/FlowMeasure.h"
#include "flow-sdk/HttpClient.h"
#include "flow-sdk/Logger.h"
#include "log/LogDecorator.h"
#include "log/NullLogger.h"

namespace FlowSdk::Plugin {
    struct SdkFactory::SdkFactoryImpl {
        auto CreateDataListeners() -> std::unique_ptr<Plugin::ConcreteEventListeners<const nlohmann::json&>>
        {
            auto dataListeners = std::make_unique<Plugin::ConcreteEventListeners<const nlohmann::json&>>();
            dataListeners->Add(std::make_shared<Api::FlightInformationRegionDataParser>(GetFirs(), GetLogger()));
            dataListeners->Add(std::make_shared<Api::EventDataParser>(GetEvents(), GetFirs(), GetLogger()));
            dataListeners->Add(std::make_shared<Api::FlowMeasureDataParser>(
                    std::make_unique<Api::FlowMeasureFilterParser>(GetLogger()),
                    std::make_unique<Api::FlowMeasureMeasureParser>(GetLogger()), GetFlowMeasures(), GetEvents(),
                    GetFirs(), GetLogger()
            ));

            return dataListeners;
        }

        auto CreateApiDataScheduler() -> std::shared_ptr<Api::ApiDataScheduler>
        {
            return std::make_shared<Api::ApiDataScheduler>(
                    std::make_unique<Api::ApiDataDownloader>(std::move(httpClient), CreateDataListeners(), GetLogger())
            );
        }

        auto CreateLogger() -> std::shared_ptr<Log::Logger>
        {
            if (logger == nullptr) {
                logger = std::make_unique<Log::NullLogger>();
            }

            return std::make_shared<Log::LogDecorator>(std::move(logger));
        }

        auto GetLogger() -> std::shared_ptr<Log::Logger>
        {
            if (!wrappedLogger) {
                wrappedLogger = CreateLogger();
            }

            return wrappedLogger;
        }

        auto GetFirs() -> std::shared_ptr<
                Api::ConcreteStringIdentifiedApiElementCollection<FlightInformationRegion::FlightInformationRegion>>
        {
            if (!firs) {
                firs = std::make_shared<Api::ConcreteStringIdentifiedApiElementCollection<
                        FlightInformationRegion::FlightInformationRegion>>();
            }

            return firs;
        }

        auto GetEvents() -> std::shared_ptr<Api::ConcreteApiElementCollection<Event::Event>>
        {
            if (!events) {
                events = std::make_shared<Api::ConcreteApiElementCollection<Event::Event>>();
            }

            return events;
        }

        auto GetFlowMeasures()
                -> std::shared_ptr<Api::ConcreteStringIdentifiedApiElementCollection<FlowMeasure::FlowMeasure>>
        {
            if (!flowMeasures) {
                flowMeasures =
                        std::make_shared<Api::ConcreteStringIdentifiedApiElementCollection<FlowMeasure::FlowMeasure>>();
            }

            return flowMeasures;
        }

        auto GetEventBus() -> std::shared_ptr<EventBus::InternalEventBus>
        {
            if (!eventBus) {
                eventBus = std::make_shared<EventBus::InternalEventBus>();
            }

            return eventBus;
        }

        // For performing HTTP requests
        std::unique_ptr<Http::HttpClient> httpClient = nullptr;

        // For logging things that happen
        std::unique_ptr<Log::Logger> logger = nullptr;

        // A wrapper around the logger
        std::shared_ptr<Log::Logger> wrappedLogger = nullptr;

        // For broadcasting events
        std::shared_ptr<EventBus::InternalEventBus> eventBus = nullptr;

        // All the FIRs
        std::shared_ptr<
                Api::ConcreteStringIdentifiedApiElementCollection<FlightInformationRegion::FlightInformationRegion>>
                firs;

        std::shared_ptr<Api::ConcreteApiElementCollection<Event::Event>> events;

        std::shared_ptr<Api::ConcreteStringIdentifiedApiElementCollection<FlowMeasure::FlowMeasure>> flowMeasures;
    };

    SdkFactory::SdkFactory() : impl(std::make_unique<SdkFactoryImpl>())
    {}

    SdkFactory::~SdkFactory() = default;

    auto SdkFactory::Build() -> SdkFactory
    {
        return {};
    }

    auto SdkFactory::WithHttpClient(std::unique_ptr<Http::HttpClient> http) -> SdkFactory&
    {
        if (impl->httpClient) {
            throw SdkConfigurationException("Duplicate HTTP client provided");
        }

        impl->httpClient = std::move(http);
        return *this;
    }

    auto SdkFactory::WithLogger(std::unique_ptr<Log::Logger> log) -> SdkFactory&
    {
        if (impl->logger) {
            throw SdkConfigurationException("Duplicate logger provided");
        }

        impl->logger = std::move(log);
        return *this;
    }

    auto SdkFactory::Instance() -> std::unique_ptr<Sdk>
    {
        if (!impl->httpClient) {
            throw SdkConfigurationException("No http client provided");
        }

        return std::make_unique<ConcreteSdk>(
                std::shared_ptr<void>(impl->CreateApiDataScheduler()), impl->GetEventBus()
        );
    }
}// namespace FlowSdk::Plugin
