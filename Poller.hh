
#pragma once 

#include "EventDemultiplexer.hh"

struct pollfd;

namespace Wood 
{
class EventLoop;

class Poller : public EventDemultiplexer
{
public:
	Poller(EventLoop* loop);
	virtual ~Poller() = default;
	virtual void poll(int timeout, EventHandlerList* activeEventHandlers );
	virtual void update(EventHandler* handler);
	virtual void remove(EventHandler* handler);
private:
	void fillActiveHandlers(int numDescriptors, EventHandlerList* activeEventHandlers);
	using pollfds = std::vector<struct pollfd>;
	pollfds pfds;
};
}