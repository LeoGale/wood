#include <poll.h>
#include <unistd.h>
#include <iostream>
#include <assert.h>
#include <sstream>

#include "Poller.hh"
#include "wood/net/EventHandler.hh"

namespace wood {

namespace utils {
	std::string pollfd_to_string(pollfd & pfd) {
        std::ostringstream ss;
        ss << "{ fd ";
        ss << pfd.fd;
        ss <<" events ";
        ss << pfd.events;
        ss << " revents ";
        ss << pfd.revents;
        ss <<" }";
        return ss.str().c_str();
    }
}

Poller::Poller(EventLoop* loop)
:EventDemultiplexer(loop)
{

}

void Poller::poll(int timeout, EventHandlerList* activeEventHandlers )
{
	int numDescriptors = ::poll(&*pfds.begin(), pfds.size(), timeout);

	if(numDescriptors > 0)
	{
		fillActiveHandlers(numDescriptors, activeEventHandlers);
	}
	else if(numDescriptors == 0)
	{
		std::cout << "EventLoop::loop timeout occurs." << std::endl;
	}
	else 
	{
		std::cout <<"EventLoop::loop error occurs " << std::endl;
	}
}

void Poller::update(EventHandler* handler)
{
	std::cout <<"Poller::update " << handler->eventsStr() << std::endl;
	assertInLoopThread();
	auto handleIter = handlers_.find(handler->fd());
	if(handleIter == std::end(handlers_))
	{
		struct pollfd pfd;
		pfd.fd = handler->fd();
		pfd.events = handler->events();
		pfd.revents = 0;
		std::cout <<"Poller::update add a new one " << utils::pollfd_to_string(pfd)  << std::endl;
		pfds.push_back(pfd);
		handler->setIndex(pfds.size() - 1);
		handlers_.insert(std::make_pair(handler->fd(), handler));
	}
	else 
	{
		int index = handleIter->second->index();
		assert(index < pfds.size());
		struct pollfd& pfd = pfds[index];
		pfd.events = handleIter->second->events();
		std::cout <<"Poller::update update an exist one to " <<utils::pollfd_to_string(pfds[index]) << std::endl;

		if(pfd.events == EventHandler::NoneEvent)
		{
			pfd.fd = -handleIter->second->fd() - 1;//-1 when fd is zero
		}
	}
}

void Poller::remove(EventHandler* handler)
{
	assertInLoopThread();
	std::cout <<"Poller::remove " << handler->eventsStr() << std::endl;
	assert(handler);
	assert(handler->events() == EventHandler::NoneEvent);
	assert(handlers_.find(handler->fd()) != std::end(handlers_));

	int index = handler->index();
	assert( index < pfds.size());
	const struct pollfd & pfd = pfds[index];
	assert(pfd.fd < 0);
	assert(pfd.fd == (-handler->fd() -1));
	assert(pfd.events == handler->events());

	handlers_.erase(handler->fd());

	if(index == pfds.size() - 1)
	{
		pfds.pop_back();
	}
	else 
	{
		int endFd = pfds.back().fd;

		std::iter_swap(pfds.begin()+index, pfds.end()-1);

		if(endFd < 0)
		{
			endFd = -endFd -1;
		}
		handlers_[endFd]->setIndex(index);
		pfds.pop_back();
	}
}

void Poller::fillActiveHandlers(int numDescriptors, EventHandlerList* activeEventHandlers)
{
	for(auto & pfd : pfds)
	{
		if(numDescriptors <= 0)
		{
			break;
		}

		if(pfd.revents > 0)
		{
			--numDescriptors;
			auto handler = handlers_.find(pfd.fd);
			assert(handler != std::end(handlers_));
			handler->second->setRevents(pfd.revents);
			activeEventHandlers->push_back(handler->second);
		}
	}
}

}
