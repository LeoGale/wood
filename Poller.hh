#include <vector>
#include <map>

struct pollfd;

namespace Wood {
class EventLoop;

class Poller {
public:
    Poller(EventLoop* loop);
    ~Poller();

    void poll(int timeout, std::vector<EventHandler*> *handlers);

    void update(EventHandler* handler);

    void remove(EventHandler *handler);

  private:
    EventLoop* loop_;

    void fillActiveEvHandler(int numEvents, std::vector<EventHandler *> *handlers);
    // struct pollfd
    // {
    //     int fd;        /* file descriptor */
    //     short events;  /* requested events */
    //     short revents; /* returned events */
    // };
    std::vector<pollfd> pollfds_;

    std::map<int, EventHandler*> handlers_;
};

}