#include "EventLoop.hh"
#include "Poller.hh"
#include "EventHandler.hh"

constexpr int PollTimeout = 1000;

namespace Wood {

EventLoop::EventLoop()
:quit_(false),
isLooping(false),
eventHanlding(false),
poller_(new Poller(this))
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
        poller_->poll(PollTimeout, &activeHandlers_);

        //eventHanlding = true;
        for (auto &activeHanlder : activeHandlers_)
        {
            activeHanlder->handleEvent();
        }
        //eventHanlding = false;
    }
    isLooping = false;
}

void EventLoop::stop()
{
    quit_ = true;
}

}

int main() {
    Wood::EventLoop aLoop;
    aLoop.loop();
}