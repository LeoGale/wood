#include "EPoller.hh"
//FIXME remove .. after involve cmake in project
#include "EventHandler.hh"

namespace Wood 
{
EPoller::EPoller(EventLoop* loop)
:EventDemultiplexer(loop)
{

}

void EPoller::poll(int timeout, EventHandlerList *activeEventHandlers)
{

}

void EPoller::update(EventHandler* handler)
{

}

void EPoller::remove(EventHandler* handler)
{

}

}