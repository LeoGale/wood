#include "EventLoop.hh"
#include "Poller.hh"

namespace Wood {

EventLoop::EventLoop()
:quit_(false),
isLooping(false),
eventHanlding(false)
{
}

EventLoop::~EventLoop()
{
}

void EventLoop::loop() 
{
    while(!quit_) 
    {
        isLooping = true;
        activeHandlers_.clear();
        poller_->poll(PollTimeout, activeHandlers_);

        eventHanlding = true;
        for (auto &activeHanlder : activeHandlers_)
        {
            activeHanlder->handleEvent();
        }
        eventHanlding = false;
    }
    isLooping = false;
}

void EventLoop::stop()
{
    quit_ = true;
}
eha
}