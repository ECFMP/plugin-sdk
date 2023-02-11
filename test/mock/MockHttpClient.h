#pragma once
#include "flow-sdk/HttpClient.h"
#include <gmock/gmock.h>

namespace FlowSdkTest::Http {
    class MockHttpClient : public FlowSdk::Http::HttpClient
    {
        public:
        MOCK_METHOD(FlowSdk::Http::HttpResponse, Get, (const std::string&), (override));
    };
}// namespace FlowSdkTest::Http
