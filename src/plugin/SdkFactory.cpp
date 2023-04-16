#include "flow-sdk/SdkFactory.h"
#include "ConcreteEventListeners.h"
#include "ConcreteSdk.h"
#include "ConcreteSdkEventListeners.h"
#include "api/ApiDataDownloader.h"
#include "api/ApiDataScheduler.h"
#include "api/ConcreteApiElementCollection.h"
#include "api/ConcreteStringIdentifiedApiElementCollection.h"
#include "api/EventDataParser.h"
#include "api/FlightInformationRegionDataParser.h"
#include "flow-sdk/FlightInformationRegion.h"
#include "flow-sdk/HttpClient.h"
#include "flow-sdk/Logger.h"
#include "log/LogDecorator.h"
#include "log/NullLogger.h"

namespace FlowSdk::Plugin {
    struct SdkFactory::SdkFactoryImpl {
        static auto CreateEventListeners() -> std::unique_ptr<ConcreteSdkEventListeners>
        {
            return std::make_unique<ConcreteSdkEventListeners>(
                    std::make_unique<ConcreteEventListeners<FlowMeasure::FlowMeasure>>(),
                    std::make_unique<ConcreteEventListeners<FlowMeasure::FlowMeasure>>(),
                    std::make_unique<ConcreteEventListeners<FlowMeasure::FlowMeasure>>(),
                    std::make_unique<ConcreteEventListeners<FlowMeasure::FlowMeasure>>(),
                    std::make_unique<ConcreteEventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>>()
            );
        }

        auto CreateDataListeners() -> std::unique_ptr<Plugin::ConcreteEventListeners<const nlohmann::json&>>
        {
            auto dataListeners = std::make_unique<Plugin::ConcreteEventListeners<const nlohmann::json&>>();
            dataListeners->Add(std::make_shared<Api::FlightInformationRegionDataParser>(GetFirs(), GetLogger()));
            dataListeners->Add(std::make_shared<Api::EventDataParser>(GetEvents(), GetFirs(), GetLogger()));

            return dataListeners;
        }

        auto CreateApiDataScheduler() -> std::unique_ptr<Api::ApiDataScheduler>
        {
            return std::make_unique<Api::ApiDataScheduler>(
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

        // For performing HTTP requests
        std::unique_ptr<Http::HttpClient> httpClient = nullptr;

        // For logging things that happen
        std::unique_ptr<Log::Logger> logger = nullptr;

        // A wrapper around the logger
        std::shared_ptr<Log::Logger> wrappedLogger = nullptr;

        // All the FIRs
        std::shared_ptr<
                Api::ConcreteStringIdentifiedApiElementCollection<FlightInformationRegion::FlightInformationRegion>>
                firs;

        std::shared_ptr<Api::ConcreteApiElementCollection<Event::Event>> events;
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

        return std::make_unique<ConcreteSdk>(impl->CreateApiDataScheduler(), impl->CreateEventListeners());
    }
}// namespace FlowSdk::Plugin
