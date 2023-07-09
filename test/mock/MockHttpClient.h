#pragma once
#include "ECFMP/http/HttpClient.h"
#include <gmock/gmock.h>

namespace ECFMPTest::Http {
    class MockHttpClient : public ECFMP::Http::HttpClient
    {
        public:
        MOCK_METHOD(ECFMP::Http::HttpResponse, Get, (const std::string&), (override));
    };
}// namespace ECFMPTest::Http
