#pragma once

namespace FlowSdk::Api {
    /**
     * A class for sourcing data from the API
     */
    class ApiDataSource
    {
        public:
        [[nodiscard]] auto GetData() -> std::string;
    };
}// namespace FlowSdk::Api
