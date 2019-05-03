//
//  Thread.hh
//  
//
//  Created by leo on 2019/4/24.
//
#pragma once

#include <string>
#include <functional>
#include <atomic>

namespace tornado {
class Thread final {
public:
    using ThreadFunc = std::function<void()>;
    Thread(const std::string& name, ThreadFunc && func);
    ~Thread();
    Thread(Thread && theOther); = delete;
    Thread& (const Thread&) = delete;
    Thread& operator= (const Thread &) = delete;
    Thread& operator=(Thread&&) = delete;
    void start();
    void detach();
    int join();
    pid_t tid();
    const std::string& name();
private:
    void setDefaultName(int32_t threadNum)
    bool started_;
    bool joined_;
    std::string name_;
    ThreadFunc func_;
    pid_t tid_;
    pthread_t pthreadId_;
    static std::atomic<int32_t> numCreated_;
};
}
