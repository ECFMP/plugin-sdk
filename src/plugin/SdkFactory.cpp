#include "flow-sdk/SdkFactory.h"
#include "ConcreteEventListeners.h"
#include "ConcreteSdk.h"
#include "ConcreteSdkEventListeners.h"
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
                    std::make_unique<ConcreteEventListeners<FlowMeasure::FlowMeasure, FlowMeasure::FlowMeasure>>());
        }

        auto CreateLogger() -> std::shared_ptr<Log::Logger>
        {
            if (logger == nullptr) {
                logger = std::make_unique<Log::NullLogger>();
            }

            return std::make_shared<Log::LogDecorator>(std::move(logger));
        }

        // For performing HTTP requests
        std::unique_ptr<Http::HttpClient> httpClient;

        // For logging things that happen
        std::unique_ptr<Log::Logger> logger;
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

        return std::make_unique<ConcreteSdk>(impl->CreateEventListeners());
    }
}// namespace FlowSdk::Plugin
