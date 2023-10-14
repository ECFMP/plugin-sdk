#pragma once

namespace ECFMP::Thread {
    class ThreadPool
    {
        public:
        ThreadPool();
        ~ThreadPool();

        void Schedule(const std::function<void()>& function);

        private:
        struct Impl;
        std::unique_ptr<Impl> impl;
    };
}// namespace ECFMP::Thread
