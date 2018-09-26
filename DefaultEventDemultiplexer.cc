#include <stdlib.h> //For getenv
#include <poll.h>

//FIXME remove .. after involve cmake in project
#include "EventDemultiplexer.hh"
#include "Poller.hh"
#include "EventLoop.hh"
#include "EPoller.hh"

namespace Wood 
{
	EventDemultiplexer* EventDemultiplexer::createDefaultPoller(EventLoop* loop)
	{
		//return new Poller(loop);
		if(::getenv("Event_Demultiplexer_Mode") == "EPoll")
		{
			return new EPoller(loop);
		}
		else
		{
			//FIXME default by poll
			return new Poller(loop);
		}
	}
}