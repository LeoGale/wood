//
//  Thread.cc
//
//
//  Created by leo on 2019/4/24.
//

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/prctl.h>

#include <assert.h>

#include <future>
#include <iostream>

#include "Thread.hh"
#include "CurrentThread.hh"

namespace wood 
{

namespace details
{
pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void afterFork()
{
  wood::CurrentThread::t_tid_ = 0;
  wood::CurrentThread::t_name_ = "main";
  CurrentThread::tid();
  std::cout <<"after fork.." << std::endl;
}

class ThreadNameInitializer
{
 public:
  ThreadNameInitializer()
  {
    std::cout <<"thread name initializer" << std::endl;
    wood::CurrentThread::t_name_ = "main";
    wood::CurrentThread::tid();
    pthread_atfork(NULL, NULL, &afterFork);
  }
};

ThreadNameInitializer init;

class ThreadData{
public:
    using ThreadFunc = Thread::ThreadFunc;
    ThreadData(const std::string& name, ThreadFunc && func, int* tid, std::promise<void>* promise)
    :name_(name),
    func_(std::forward<ThreadFunc>(func)),
    tid_(tid),
    promise_(promise)
    {
    }

    void run()
    {
        *tid_ = gettid();
        tid_ = nullptr;

        promise_->set_value();

        wood::CurrentThread::t_name_ = name_.empty() ? "woodThread" : name_.c_str();

        ::prctl(PR_SET_NAME, wood::CurrentThread::t_name_);

        try {
            func_();
            wood::CurrentThread::t_name_ = "finished";
        }
        catch ( std::exception &e)
        {
            wood::CurrentThread::t_name_ = "crashed";
            fprintf(stderr, "exception caught in thread %s\n.", name_.c_str());
            fprintf(stderr, "reason: %s\n", e.what());
            abort();
        }
        catch(...)
        {
            wood::CurrentThread::t_name_ = "crashed";
            fprintf(stderr, "unknown exception caught in thread %s\n", name_.c_str());
            throw;
        }
    }
private:
    std::string name_;
    ThreadFunc func_;
    pid_t* tid_;
    std::promise<void>* promise_;
};

void* startThread(void* data)
{
    ThreadData* threadData = static_cast<ThreadData*>(data);
    threadData->run();
    return nullptr;
}
}

void CurrentThread::cacheTid()
{
    if(t_tid_ == 0)
    {
        t_tid_ = wood::details::gettid();
    }
}

bool CurrentThread::isMainThread()
{
    return (tid() == ::getpid());
}

std::atomic<int32_t> Thread::numCreated_ = ATOMIC_VAR_INIT(0);

Thread::Thread(const std::string& name, ThreadFunc&& func)
:started_(false),
 joined_(false),
 name_(name),
 func_(std::forward<ThreadFunc>(func))
 {
      //using num++ or ++num is not thread-safe. why?
     //setDefaultName();
     setDefaultName();
 }

Thread::~Thread()
{
  if(started_ && !joined_)
  {
    join();
  }
}
void Thread::start()
{
    assert(!started_);

    started_ = true;

    std::promise<void> aPromise;

    std::unique_ptr<details::ThreadData> aData(new details::ThreadData(name_, std::move(func_), &tid_, &aPromise));

    if(pthread_create(&pthreadId_, nullptr, &details::startThread, aData.get()))
    {
        started_ = false;
        std::cout << "Thread::start failed in pthread_create." << std::endl;
    }
    else
    {
        aPromise.get_future().wait();
    }
    assert(tid_ > 0);
}


int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, nullptr);
}

void Thread::setDefaultName()
{
  int threadNum = ++numCreated_;
    if(name_.empty())
    {
        name_.append("Thread"+std::to_string(threadNum));
    }
    (void)threadNum;
}

}
