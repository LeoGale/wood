
#pragma once 

#include "wood/net/EventDemultiplexer.hh"

struct pollfd;

namespace wood 
{
class EventLoop;

class Poller : public EventDemultiplexer
{
public:
	Poller(EventLoop* loop);
	virtual ~Poller() = default;
	virtual void poll(int timeout, EventHandlerList* activeEventHandlers ) override;
	virtual void update(EventHandler* handler) override;
	virtual void remove(EventHandler* handler) override;
private:
	void fillActiveHandlers(int numDescriptors, EventHandlerList* activeEventHandlers);
	using pollfds = std::vector<struct pollfd>;
	pollfds pfds;
};
}
