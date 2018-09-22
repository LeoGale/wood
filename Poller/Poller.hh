//FIXME remove .. after add cmake
#include "../EventDemultiplexer.hh"

struct pollfd;

namespace Wood 
{
class EventLoop;
class EventHandler;

class Poller : public EventDemultiplexer {
public:
    Poller(EventLoop* loop);
    virtual ~Poller() = default;

    virtual void poll(int timeout, EventHandlers *handlers);

    virtual void update(EventHandler* handler);

    virtual void remove(EventHandler *handler);

  private:

    void fillActiveEvHandler(int numEvents, EventHandlers *handlers);
    // struct pollfd
    // {
    //     int fd;        /* file descriptor */
    //     short events;  /* requested events */
    //     short revents; /* returned events */
    // };
    std::vector<pollfd> pollfds_;
};

}