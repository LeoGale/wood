#include <poll.h>
#include <iostream>
#include <assert.h>

#include "Poller.hh"
//FIXME remove .. after involve cmake in project
#include "../EventHandler.hh"

namespace Wood {
Poller::Poller(EventLoop *loop)
: EventDemultiplexer(loop)
{

}

void Poller::poll(int timeout, EventHandlers *handlers)
{
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeout);
    int savedError = errno;
    if(numEvents > 0) 
    {
        std::cout << "Poller::poll there are " << numEvents << " happens." << std::endl;
        fillActiveEvHandler(numEvents, handlers);
    }
    else if ( numEvents == 0) 
    {
        std::cout <<"Poller::poll nothing happens" << std::endl;
    }
    else 
    {
        if (savedError != EINTR)
        {
            errno = savedError;
            std::cerr << "Poller::poll " << std::endl;
        }
    }
}

void Poller::update(EventHandler *handler)
{
    auto aHandler = handlers_.find(handler->fd());
    if (aHandler == std::end(handlers_))
    {
        handlers_.insert(std::pair<int, EventHandler*>(handler->fd(), handler));
        std::cout << "Poller::update add eventHandler " << handler->eventsStr() << std::endl;
    }
    else 
    {
        aHandler->second->setEvents(handler->events());
    }
}

void Poller::remove(EventHandler *handler)
{
    auto aHandler = handlers_.find(handler->fd());

    if (aHandler != std::end(handlers_)) 
    {
        handlers_.erase(handler->fd());
    }
}

void Poller::fillActiveEvHandler(int numEvents, EventHandlers *handlers)
{
    for (auto &pfd : pollfds_)
    {
        if (numEvents <= 0) 
        {
            break;
        }
        
        if (pfd.revents > 0)
        {
            numEvents--;
            auto handler = handlers_.find(pfd.fd);
            assert(handler == std::end(handlers_));
            EventHandler * evHandler = handler->second;
            evHandler->setRevents(pfd.revents);
            handlers->push_back(evHandler);
        }
    }
}

}