#include "ApiDataDownloader.h"
#include "flow-sdk/HttpClient.h"
#include "flow-sdk/Logger.h"
#include "nlohmann/json.hpp"
#include "plugin/InternalEventListeners.h"

namespace ECFMP::Api {

    const std::string PLUGIN_API_DATA_URL = "https://ecfmp.vatsim.net/api/v1/plugin?deleted=1";

    ApiDataDownloader::ApiDataDownloader(
            std::unique_ptr<Http::HttpClient> httpClient,
            std::unique_ptr<Plugin::InternalEventListeners<const nlohmann::json&>> listeners,
            std::shared_ptr<Log::Logger> logger
    )
        : httpClient(std::move(httpClient)), listeners(std::move(listeners)), logger(std::move(logger))
    {
        assert(this->httpClient && "Http client not set in ApiDataDownloader");
        assert(this->listeners && "Listeners not set in ApiDataDownloader");
        assert(this->logger && "Logger not set in ApiDataDownloader");
    }
    ApiDataDownloader::~ApiDataDownloader() = default;

    void ApiDataDownloader::DownloadData()
    {
        // Get the API response
        logger->Info("Downloading data");
        const auto apiResponse = httpClient->Get(PLUGIN_API_DATA_URL);
        if (apiResponse.statusCode != 200L) {
            logger->Error(
                    "Failed to download data from ECFMP, status code was " + std::to_string(apiResponse.statusCode)
            );
            return;
        }

        // Check it's valid JSON
        nlohmann::json parsedData;
        try {
            parsedData = nlohmann::json::parse(apiResponse.body);
        }
        catch (std::exception& exception) {
            logger->Error("Failed to parse data from ECFMP, was not valid JSON");
            return;
        }

        if (!parsedData.is_object()) {
            logger->Error("Failed to parse data from ECFMP, was not a JSON object");
            return;
        }

        // Disseminate the JSON to any listeners
        listeners->OnEvent(parsedData);
    }
}// namespace ECFMP::Api
