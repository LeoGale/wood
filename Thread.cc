//
//  Thread.cc
//  
//
//  Created by leo on 2019/4/24.
//

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include <future>

#include "Thread.hh"
#include "CurrentThread.hh"

namespace tornado
{
    
namespace details
{
pid_t gettid()
{
    return static_cast<pid_t>(::syscall(SYS_gittid));
}
    
class ThreadData{
public:
    using ThreadFunc = Thread::ThreadFunc;
    ThreadData(const char* name, ThreadFunc && func, int* tid, std::promise<void>* promise)
    :name_(name),
    func_(std::forward<ThreadFunc>(func)),
    tid_(tid),
    promise_(promise)
    {
    }
    
    void run()
    {
        *tid_ = CurrentThread::gettid();
        tid_ = nullptr;
        promise_->set_value();
        
        ::prctl(PR_SET_NAME, tornado::CurrentThread::t_name_);
        
        try {
            func_();
            //tornado::CurrentThread::t_name_ = "finished";
        }
        catch ( std::exception &e)
        {
            //tornado::CurrentThread::t_name_ = "crashed";
            fprintf(stderr, "exception caught in thread %s\n.", name_.c_str());
            fprintf(stderr, "reason: %s\n", e.what());
            abort();
        }
        catch(...)
        {
            //tornado::CurrentThread::t_name_ = "crashed";
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
    ThreadData* data = static_cast<ThreadData*>(data);
    data->run();
    return nullptr;
}
}
    
void CurrentThread::cacheTid()
{
    if(t_tid_ == 0)
    {
        t_tid_ = tornado::details::gettid();
    }
}
    
bool CurrentThread::isMainThread()
{
    return (tid() == ::getpid());
}
    
Thread::Thread(const std::string& name, ThreadFunc&& func)
:started_(false),
 joined_(false),
 name_(name),
 func_(std::forward<ThreadFunc>(func)){
     setDefaultName(numCreated_++);
}

void Thread::start()
{
    assert(!started_);
    
    started_ = true;
    
    std::promise<void> aPromise;
    
    std::unique_ptr<ThreadData> aData(new ThreadData(name_.c_str(), std::move(func_), &tid_, &aPromise));
    
    if(pthread_create(&pthreadId_, nullptr, &details::startThread, aData.get()))
    {
        started_ = false;
        std::cerr << "Thread::start failed in pthread_create." << std::endl;
    }
    else
    {
        aPromise.get_future().wait();
    }
    assert(tid_ > 0);
}


int Thread::join()
{
    assert(started);
    assert(!joined);
    joined = true;
    return pthread_join(pthreadId_, nullptr);
}
    
void Thread::setDefaultName(int threadNum)
{
    if(name_.empty())
    {
        name_.append("Thread"+std::to_string(num));
    }
    (void)threadNum;
}
    
}
