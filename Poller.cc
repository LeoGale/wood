#include <poll.h>

#include "Poller.hh"
#include "EventLoop.hh"

namespace Wood {
Poller::Poller(EventLoop *loop)
    : loop_(loop)
{
}

Poller::~Poller() {

}

void Poller::poll(int timeout, std::vector<EventHandler*> *handlers)
{
    int numEvents = ::poll(pollfds_.begin(), pollfds_.size(), timeout);
    int savedError = errno;
    if(numEvents > 0) 
    {
        std::cout << "Poller::poll there are " << numEvents << " happens." << std::endl;
        fillActiveEvHandler(numEvents, handlers);
    }
    else if ( numEvents == 0) {
        std::cout <<"Poller::poll nothing happens" << std::endl;
    }
    else {
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
        handlers_.insert(std::make_pair<int, EventHandler>(handler->fd(), handler));
        std::cout << "Poller::update add eventHandler " << handler->to_string() << std::endl;
    }
    else 
    {
        aHandler->setEvents(handler->events());
    }
}

void Poller::remove(EventHandler *handler)
{
    auto aHandler = handlers_.find(handler->fd());

    if (aHandler != std::end(handlers_)) 
    {
        handlers_.erase(aHandler->fd());
    }
}

void Poller::fillActiveEvHandler(int numEvents, std::vector<EventHandler *> *handlers)
{
    for (auto &pfd : pollfds_)
    {
        if (numEvents <= 0) 
        {
            break;
        }
        
        if (pfd.reevents > 0)
        {
            numEvents--;
            auto handler = handlers_.find(pfd.fd);
            assert(handler == std::end(handlers_));
            handler->setReevents(pfd.reevents);
            handlers.push_back(handler->second)
        }
    }
}

}