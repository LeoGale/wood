#include "EventHandler.hh"

namespace Wood {
constexpr int EventHandler::NoneEvent = 0;
constexpr int EventHandler::ReadEvent = POLLIN | POLLPRI;
constexpr int EventHandler::WriteEvent = POLLOUT;

EventHandler::EventHandler(int fd)
:fd_(),
events(NoneEvent),
reevents(0),
index_(-1)
{

}

EventHandler::~EventHandler
{
    
}

void EventHandler::handleEvent()
{

}

std::string EventHandler::eventsStr() const
{
    return eventsToString(fd(), events());
}

std::string EventHandler::reeventsStr() const
{
    return eventsToString(fd(), reevents());
}

std::string eventsToString(int fd, int ev) const {
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