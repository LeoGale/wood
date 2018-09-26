#include "EventDemultiplexer.hh"
#include "EventLoop.hh"


namespace Wood 
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