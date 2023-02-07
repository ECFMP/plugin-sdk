#pragma once

namespace FlowSdk::Http {

    /**
     * Represents the HTTP response.
     */
    using HttpResponse = struct HttpResponse {
        // The status code
        long statusCode;

        // The body of the response
        std::string body;
    };

    /**
     * A class that plugins consuming this SDK **must** implement to
     * allow downloading of ECFMP data.
     */
    class HttpClient
    {
        public:
        virtual ~HttpClient() = default;
        /**
         * Performs a GET request to the provided URL and returns the response.
         *
         * May throw an exception if an error occurs.
         */
        [[nodiscard]] virtual auto Get(const std::string& url) -> HttpResponse = 0;
    };
}// namespace FlowSdk::Http
