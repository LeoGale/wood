#pragma once 
#include <map>
#include <vector>

namespace wood 
{
class EventLoop;
class EventHandler;

using EventHandlerList = std::vector<EventHandler*> ;

class EventDemultiplexer
{
public:
	EventDemultiplexer(EventLoop* loop);
	virtual ~EventDemultiplexer();

	static EventDemultiplexer* createDefaultPoller(EventLoop* loop);
	virtual void poll(int timeout, EventHandlerList* activeEventHandlers ) = 0;
	virtual void update(EventHandler* handler) = 0;
	virtual void remove(EventHandler* handler) = 0;
protected:
	void assertInLoopThread();
	std::map<int, EventHandler*> handlers_;
private:
	EventLoop* loop_;
};
}
