#include "EventDemultiplexer.hh"
#include "EventLoop.hh"


namespace wood 
{
	EventDemultiplexer::EventDemultiplexer(EventLoop* loop)
	:loop_(loop) {

	}
	EventDemultiplexer::~EventDemultiplexer() {}

	void EventDemultiplexer::assertInLoopThread() 
	{
		loop_->assertInLoopThread();
	}
}
