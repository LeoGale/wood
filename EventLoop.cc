#include <sys/eventfd.h>
#include <iostream>
#include <unistd.h>
#include <thread>

#include "EventLoop.hh"
#include "Poller.hh"
#include "EventHandler.hh"

namespace Wood {

namespace Details {
    constexpr int PollTimeout = 1000;
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
isLooping(false),
eventHanlding(false),
wakefd_(Details::createEventfd()),
eventHandler_(new EventHandler(wakefd_, this)),
poller_(new Poller(this))
{
    eventHandler_->setReadCallback(std::bind(&EventLoop::handleRead, this));
    eventHandler_->enableReading();
}

EventLoop::~EventLoop()
{
    eventHandler_->disableAll();
    eventHandler_->remove();
    ::close(wakefd_);
}

void EventLoop::loop() 
{
    while(!quit_) 
    {
        isLooping = true;
        activeHandlers_.clear();
        poller_->poll(Details::PollTimeout, &activeHandlers_);

        std::cout <<"EventLoop::loop time " << time(nullptr) << std::endl;
        eventHanlding = true;
        for (auto &activeHanlder : activeHandlers_)
        {
            activeHanlder->handleEvent();
        }

        runPendingTasks();

        eventHanlding = false;
    }
    isLooping = false;
}

void EventLoop::stop()
{
    quit_ = true;
}

void EventLoop::weak()
{
    int64_t data = 1;
    int nwrite = ::write(wakefd_, &data, sizeof data);
    if(nwrite < sizeof data)
    {
        std::cerr <<"EventLoop::weak write nwrite instead of " << sizeof data << std::endl;
    }
}

void EventLoop::runInLoop(Task task) {
    if(false /*isInLoopThread*/)
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
    weak();
}

void EventLoop::updateEventHandler(EventHandler* eventHandler)
{
    poller_->update(eventHandler);
}

void EventLoop::removeEventHandler(EventHandler* eventHandler)
{
    poller_->remove(eventHandler);
}

void EventLoop::handleRead()
{
    std::cout <<"EventLoop::handleRead" << std::endl;
}

void EventLoop::runPendingTasks()
{
    std::cout <<"TRACE EventLoop::runPendingTasks" << std::endl;
    for(auto& task : tasks)
    {
        task();
    }
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