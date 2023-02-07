#include "plugin/ConcreteEventListeners.h"
#include "flow-sdk/EventListener.h"
#include "flow-sdk/EventListenerFilter.h"

namespace FlowSdkTest::Plugin {

    class ConcreteEventListenersTest : public testing::Test
    {
        public:
        FlowSdk::Plugin::ConcreteEventListeners<std::string, std::string> listeners;
    };

    class ListenerImpl : public FlowSdk::Plugin::EventListener<std::string, std::string>
    {
        public:
        ListenerImpl(std::string expected1, std::string expected2)
            : expected1(std::move(expected1)), expected2(std::move(expected2))
        {}

        void OnEvent(const std::string& string1, const std::string& string2) override
        {
            if (called) {
                throw std::exception("More than one call");
            }

            actual1 = string1;
            actual2 = string2;
            called = true;
        }

        void AssertCalled()
        {
            ASSERT_TRUE(called);
            ASSERT_EQ(expected1, actual1);
            ASSERT_EQ(expected2, actual2);
        }

        void AssertNotCalled()
        {
            ASSERT_FALSE(called);
        }

        private:
        std::string expected1;
        std::string expected2;
        std::string actual1;
        std::string actual2;
        bool called = false;
    };

    class FilterImpl : public FlowSdk::Plugin::EventListenerFilter<std::string, std::string>
    {
        public:
        FilterImpl(bool shouldPass, std::string expected1, std::string expected2)
            : shouldPass(shouldPass), expected1(std::move(expected1)), expected2(std::move(expected2))
        {}

        auto Passes(const std::string& string1, const std::string& string2) -> bool override
        {
            if (called) {
                throw std::exception("More than one call");
            }

            actual1 = string1;
            actual2 = string2;
            called = true;

            return shouldPass;
        }

        void AssertCalled()
        {
            ASSERT_TRUE(called);
            ASSERT_EQ(expected1, actual1);
            ASSERT_EQ(expected2, actual2);
        }

        void AssertNotCalled()
        {
            ASSERT_FALSE(called);
        }

        private:
        bool shouldPass;
        std::string expected1;
        std::string expected2;
        std::string actual1;
        std::string actual2;
        bool called = false;
    };

    TEST_F(ConcreteEventListenersTest, ItAddsListeners)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        listeners.Add(listener);
        listeners.OnEvent("abc", "def");

        listener->AssertCalled();
    }

    TEST_F(ConcreteEventListenersTest, ItDoesntAddDuplicateListeners)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        listeners.Add(listener);
        listeners.Add(listener);
        listeners.Add(listener);
        listeners.OnEvent("abc", "def");

        listener->AssertCalled();
    }

    TEST_F(ConcreteEventListenersTest, ItAddsListenersWithAPassingFilter)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        auto filter = std::make_shared<FilterImpl>(true, "abc", "def");
        listeners.Add(listener, filter);
        listeners.OnEvent("abc", "def");

        listener->AssertCalled();
        filter->AssertCalled();
    }

    TEST_F(ConcreteEventListenersTest, ItDoesntAddDuplicateListenerWithPassingFilter)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        auto filter = std::make_shared<FilterImpl>(true, "abc", "def");
        listeners.Add(listener, filter);
        listeners.Add(listener, filter);
        listeners.Add(listener, filter);
        listeners.Add(listener, filter);
        listeners.OnEvent("abc", "def");

        listener->AssertCalled();
        filter->AssertCalled();
    }

    TEST_F(ConcreteEventListenersTest, ItAddsListenersWithAFailingFilter)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        auto filter = std::make_shared<FilterImpl>(false, "abc", "def");
        listeners.Add(listener, filter);
        listeners.OnEvent("abc", "def");

        listener->AssertNotCalled();
        filter->AssertCalled();
    }

    TEST_F(ConcreteEventListenersTest, ItRemovesAListener)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        listeners.Add(listener);
        listeners.Remove(listener);
        listeners.OnEvent("abc", "def");

        listener->AssertNotCalled();
    }

    TEST_F(ConcreteEventListenersTest, ItRemovesAListenerAndFilter)
    {
        auto listener = std::make_shared<ListenerImpl>("abc", "def");
        auto filter = std::make_shared<FilterImpl>(true, "abc", "def");
        listeners.Add(listener, filter);
        listeners.Remove(listener);
        listeners.OnEvent("abc", "def");

        listener->AssertNotCalled();
        filter->AssertNotCalled();
    }
}// namespace FlowSdkTest::Plugin
