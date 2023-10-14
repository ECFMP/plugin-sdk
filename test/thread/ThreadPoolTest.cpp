#include "thread/ThreadPool.h"

namespace ECFMPTest::Thread {
    class ThreadPoolTest : public ::testing::Test
    {
        public:
        ECFMP::Thread::ThreadPool threadPool;
    };

    TEST_F(ThreadPoolTest, ItRunsTask)
    {
        std::atomic<int> counter = 0;
        threadPool.Schedule([&counter]() {
            counter++;
        });
        threadPool.Schedule([&counter]() {
            counter++;
        });
        threadPool.Schedule([&counter]() {
            counter++;
        });
        threadPool.Schedule([&counter]() {
            counter++;
        });

        // Sleep for a bit to allow the tasks to run
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        EXPECT_EQ(4, counter);
    }
}// namespace ECFMPTest::Thread
