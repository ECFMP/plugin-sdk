#pragma once
#include "ECFMP/flowmeasure/CustomFlowMeasureFilter.h"
#include "Sdk.h"
#include <memory>

namespace ECFMP {
    namespace Http {
        class HttpClient;
    }// namespace Http
    namespace Log {
        class Logger;
    }
}// namespace ECFMP

namespace ECFMP::Plugin {

    class SdkConfigurationException : public std::logic_error
    {
        public:
        SdkConfigurationException(std::string message) : std::logic_error(message)
        {}
    };

    /**
     * A class that builds an SDK instance
     */
    class SdkFactory
    {
        public:
        ~SdkFactory();

        /**
         * Returns an instance of the SDK factory to start creating an SDK instance.
         */
        [[nodiscard]] static auto Build() -> SdkFactory;

        /**
         * Provides a HTTP client to the SDK, which allows it to make API calls to ECFMP. This
         * will be called asynchronously.
         *
         * This method **must** be called.
         *
         * Throws an SdkConfigurationException if a http client has already been provided.
         */
        [[nodiscard]] auto WithHttpClient(std::unique_ptr<Http::HttpClient> http) -> SdkFactory&;

        /**
         * Provides a logger to the SDK, which allows it to log what it's doing. This logger
         * should be thread-safe as it may be called asynchronously.
         *
         * Throws an SdkConfigurationException if a logger has already been provided.
         */
        [[nodiscard]] auto WithLogger(std::unique_ptr<Log::Logger> log) -> SdkFactory&;

        /**
         * Allows consumers to provide a custom filter for flow measures. This will be called for every
         * flow measure that is received from ECFMP when checking if the flow measure is applicable to
         * the given aircraft.
         */
        [[nodiscard]] auto
        WithCustomFlowMeasureFilter(const std::shared_ptr<FlowMeasure::CustomFlowMeasureFilter>& filter) -> SdkFactory&;

        /**
         * Returns the configured SDK instance.
         *
         * Throws an SdkConfigurationException if the configuration is bad.
         */
        [[nodiscard]] auto Instance() -> std::shared_ptr<Sdk>;

        private:
        SdkFactory();

        struct SdkFactoryImpl;
        std::unique_ptr<SdkFactoryImpl> impl;
    };
}// namespace ECFMP::Plugin
