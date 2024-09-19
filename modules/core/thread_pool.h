#ifndef POMEGRANATEENGINE_THREAD_POOL_H
#define POMEGRANATEENGINE_THREAD_POOL_H
#include "thread"
#include "vector"
#include "queue"
#include "functional"
#include "mutex"
#include "condition_variable"
#include "utility"
#include "iostream"

template <typename Ret, typename... Args>
class ThreadPool {
private:
    // Use std::function for FunctionBind
    using FunctionBind = std::function<Ret()>;
    std::queue<FunctionBind> _jobs;
    std::vector<std::thread> _threads;
    std::mutex _queueMutex;
    std::condition_variable _mutexCondition;
    bool _terminate = false;
    bool _autoTerminate = false;

    void searchForJob()
    {
        while (true)
        {
            FunctionBind job;

            {
                std::unique_lock<std::mutex> lock(_queueMutex);
                _mutexCondition.wait(lock, [this] {
                    return !_jobs.empty() || _terminate;
                });

                if (_terminate || (_jobs.empty() && _autoTerminate)) {
                    return;
                }

                job = std::move(_jobs.front());
                _jobs.pop();
            }

            // Execute the job
            if (job) {
                job();
            }
        }
    }

public:
    ThreadPool(bool autoTerminate = false) : _autoTerminate(autoTerminate)
    {
        _jobs = std::queue<FunctionBind>();
        _threads = std::vector<std::thread>();
    }

    ~ThreadPool() {
        stop();
    }

    void queue(std::function<Ret(Args...)> function, Args... args)
    {
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _jobs.push(std::bind(function, args...));
        }
        _mutexCondition.notify_one();
    }

    void start(uint32_t threadCount)
    {
        _terminate = false;
        _threads.reserve(threadCount);
        for (uint32_t i = 0; i < threadCount; ++i) {
            _threads.emplace_back(&ThreadPool::searchForJob, this);
        }
    }

    bool busy()
    {
        std::unique_lock<std::mutex> lock(_queueMutex);
        return !_jobs.empty();
    }

    void stop()
    {
        {
            std::unique_lock<std::mutex> lock(_queueMutex);
            _terminate = true;
        }
        _mutexCondition.notify_all();

        for (std::thread& active_thread : _threads) {
            if (active_thread.joinable()) {
                active_thread.join();
            }
        }
        _threads.clear();
    }
};


#endif //POMEGRANATEENGINE_THREAD_POOL_H
