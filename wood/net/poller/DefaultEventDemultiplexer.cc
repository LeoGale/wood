#include <stdlib.h> //For getenv
#include <poll.h>

#include "wood/net/EventLoop.hh"
#include "wood/net/EventDemultiplexer.hh"
#include "Poller.hh"
#include "EPoller.hh"

namespace wood 
{
	EventDemultiplexer* EventDemultiplexer::createDefaultPoller(EventLoop* loop)
	{
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
