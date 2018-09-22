#pragma once

#include <iostream>
#include <vector>
#include <map>

namespace Wood 
{
class EventHandler;
class EventLoop;

using EventHandlers = std::vector<EventHandler*>;

class EventDemultiplexer {
public:
	EventDemultiplexer(EventLoop* loop);
	virtual ~EventDemultiplexer() = default;

	static EventDemultiplexer* createDefaultPoller(EventLoop* loop);
	//FIXME Why pointer not reference?
	virtual void poll(int timeout, EventHandlers *handlers) = 0;
	virtual void update(EventHandler* handler) = 0;
	virtual void remove(EventHandler *handler) = 0;
	void assertInLoopThread();
protected:
	using EventHandlerMap = std::map<int, EventHandler*>;
	EventHandlerMap handlers_;

private: 
	EventLoop* loop_;
};

}