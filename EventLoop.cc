#include <sys/eventfd.h>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "EventLoop.hh"
#include "EventDemultiplexer.hh"
#include "EventHandler.hh"
#include "CurrentThread.hh"

namespace Wood {

namespace {
    constexpr int PollTimeout = 1000;

    __thread EventLoop* t_EventLoopInThread_ = nullptr;

    int createEventfd() {
        int evFd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        int errrnoSaved = errno;

        if (evFd < 0) {
            std::cerr << "FETAL ERROR createEventfd failed, error code : " << errrnoSaved << std::endl;//perror(errrnoSaved) << std::endl;
            abort();
        }
    }
}


EventLoop::EventLoop()
:quit_(false),
isLooping_(false),
eventHanlding_(false),
wakefd_(createEventfd()),
threadId_(CurrentThread::tid()),
wakeHandler_(new EventHandler(wakefd_, this)),
eventDemultiplexer_(EventDemultiplexer::createDefaultPoller(this))
{
    if(t_EventLoopInThread_)//FIXME using UNLIKELY
    {
        std::cerr <<"EventLoop::EventLoop FATAL there is a exist event loop in current thread." << std::endl;
    }
    else 
    {
        t_EventLoopInThread_ = this;
    }

    wakeHandler_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    wakeHandler_->enableReading();
}

EventLoop::~EventLoop()
{
    wakeHandler_->disableAll();
    wakeHandler_->remove();
    ::close(wakefd_);
    t_EventLoopInThread_ = nullptr;
}

void EventLoop::loop() 
{
    while(!quit_) 
    {
        isLooping_ = true;
        activeHandlers_.clear();
        eventDemultiplexer_->poll(PollTimeout, &activeHandlers_);

        std::cout <<"EventLoop::loop time " << time(nullptr) << std::endl;
        eventHanlding_ = true;
        for (auto &activeHanlder : activeHandlers_)
        {
            activeHanlder->handleEvent();
        }

        runPendingTasks();

        eventHanlding_ = false;
    }
    isLooping_ = false;
}

void EventLoop::stop()
{
    quit_ = true;
}

void EventLoop::wake()
{
    int64_t data = 1;
    int nwrite = ::write(wakefd_, &data, sizeof data);
    if(nwrite < sizeof data)
    {
        std::cerr <<"EventLoop::wake write nwrite instead of " << sizeof data << std::endl;
    }
}

void EventLoop::runInLoop(Task task) {
    if(isInLoopThread())
    {
        task();
    }
    else {
        queueInLoop(std::move(task));
    }
}

void EventLoop::queueInLoop(Task task)
{
    tasks.push_back(task);
    wake();
}

void EventLoop::updateEventHandler(EventHandler* eventHandler)
{
    eventDemultiplexer_->update(eventHandler);
}

void EventLoop::removeEventHandler(EventHandler* eventHandler)
{
    eventDemultiplexer_->remove(eventHandler);
}

void EventLoop::handleRead()
{
    std::cout <<"EventLoop::handleRead" << std::endl;
}

void EventLoop::runPendingTasks()
{
    std::cout <<"EventLoop::runPendingTasks TRACE task amount " << tasks.size() << std::endl;
    for(auto& task : tasks)
    {
        task();
    }
}

void EventLoop::assertInLoopThread()
{
    if(!isInLoopThread())
    {
        abortNotInLoopThread();
    }
}


bool EventLoop::isInLoopThread() const
{
    return threadId_ == CurrentThread::tid();
}

void EventLoop::abortNotInLoopThread()
{
    std::cout <<"EventLoop::abortNotInLoopThread FATAL not in thread " << threadId_ << ", current thread " << CurrentThread::tid() << std::endl;
#ifdef NDEBUG
    assert(false);
#endif
}

}

void threadFunc(Wood::EventLoop* loop)
{
    sleep(2);
    loop->runInLoop([](){
        for(int i = 0; i < 1; i++)
        {
            std::cout <<"thread Func " << i << std::endl;
        }
    });
}

int main() {
    Wood::EventLoop aLoop;
    std::thread aThread(threadFunc, &aLoop);
    aThread.detach();
    aLoop.loop();

    std::cout <<"main() Lopp end" << std::endl;
}


    // std::string reason(int errno) {
    //     if(errno & EINVAL) {
    //         return "EINVAL An unsupported value was specified in flags.";
    //     }

    //     if(errno & EMFILE)
    //     {
    //         return "EMFILE";
    //     }

    //     if(errno & ENFILE)
    //     {
    //         return "ENFILE"
    //     }

    //     if(errno & ENODEV)
    //     {
    //         return "ENODEV";
    //     }

    //     if(errno & ENOMEM)
    //     {
    //         return "ENOMEM";
    //     }
    // }