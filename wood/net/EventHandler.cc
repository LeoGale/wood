#include <unistd.h>
#include <poll.h>
#include <iostream>
#include <sstream>

#include "EventHandler.hh"
#include "EventLoop.hh"

namespace wood {

namespace utils {
	std::string eventsToString(int fd, int ev) {
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

const int EventHandler::NoneEvent = 0;
const int EventHandler::ReadEvent = POLLIN | POLLPRI;
const int EventHandler::WriteEvent = POLLOUT;

EventHandler::EventHandler(int fd, EventLoop* loop)
:fd_(fd),
events_(NoneEvent),
revents_(NoneEvent),
index_(-1),
loop_(loop)
{

}

void EventHandler::handleEvents()
{
	std::cout <<"EventHandler::handleEvents " << std::endl;

	if(revents_ & ReadEvent)
	{
		readCallback_();
	}
	else if(revents_ & WriteEvent)
	{
		writeCallback_();
	}
}

void EventHandler::update()
{
	loop_->updateEventHandler(this);
}

void EventHandler::remove()
{
	loop_->removeEventHandler(this);
}

std::string EventHandler::eventsStr() const
{
	return utils::eventsToString(fd_, events_);
}

std::string EventHandler::reventsStr() const
{
	return utils::eventsToString(fd_, revents_);
}

}
