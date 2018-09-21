#include "EventHandler.hh"
#include "EventLoop.hh"

#include <iostream>
#include <sstream>
#include <unistd.h>

namespace Wood {
const int EventHandler::NoneEvent = 0;
const int EventHandler::ReadEvent = POLLIN | POLLPRI;
const int EventHandler::WriteEvent = POLLOUT;

EventHandler::EventHandler(int fd, EventLoop* loop)
:fd_(fd),
events_(NoneEvent),
revents_(0),
index_(-1),
loop_(loop)
{

}

EventHandler::~EventHandler()
{
    
}

void EventHandler::handleEvent()
{
    if(revents_ & (POLLIN | POLLPRI))
    {
        std::cout <<"EventHandler::handleEvent read event-emitting." << std::endl;
        readCallback_();
    }

    if (revents_ & POLLOUT)
    {
        std::cout <<"EventHandler::handleEvent write event-emitting." << std::endl;
        writeCallback_();
    }
}

std::string EventHandler::eventsStr() const
{
    return eventsToString(fd(), events());
}

std::string EventHandler::reventsStr() const
{
    return eventsToString(fd(), revents());
}

void EventHandler::update() 
{
    loop_->updateEventHandler(this);
}

void EventHandler::remove() 
{
    loop_->removeEventHandler(this);
}

std::string EventHandler::eventsToString(int fd, int ev) const {
    std::ostringstream oss;
    oss << fd << ":";
    if (ev & POLLIN)
    {
        oss << "POLLIN ";
    }
    if (ev & POLLPRI)
    {
        oss << "POLLPRI ";
    }
    if (ev & POLLOUT)
    {
        oss << "POLLOUT ";
    }
    if (ev & POLLRDHUP)
    {
        oss << "POLLRDHUP ";
    }
    if (ev & POLLERR)
    {
        oss << "POLLERR ";
    }
    if (ev & POLLHUP)
    {
        oss << "POLLHUP ";
    }
    if (ev & POLLNVAL)
    {
        oss << "POLLNVAL ";
    }
#ifdef _XOPEN_SOURCE
    if (ev & POLLRDNORM)
    {
        oss << "POLLRDNORM ";
    }

    if (ev & POLLRDBAND)
    {
        oss << "POLLRDBAND ";
    }

    if (ev & POLLWRNORM)
    {
        oss << "POLLWRNORM ";
    }

    if (ev & POLLWRBAND)
    {
        oss << "POLLWRBAND ";
    }
#endif

    return oss.str().c_str();
}

}