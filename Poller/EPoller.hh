//FIXME remove .. after add cmake
#include "../EventDemultiplexer.hh"

namespace Wood 
{
class EPoller : public EventDemultiplexer {
public:
	EPoller(EventLoop* loop);
	virtual ~EPoller() = default;

	virtual void poll(int timeout, EventHandlers *handlers);

    virtual void update(EventHandler* handler);

    virtual void remove(EventHandler *handler);
private:
	//FIXME Epoll related.
};

}