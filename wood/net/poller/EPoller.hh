#include <vector>

#include "wood/net/EventDemultiplexer.hh"

namespace wood 
{
class EPoller : public EventDemultiplexer {
public:
    EPoller(EventLoop* loop);
    virtual ~EPoller();

    virtual void poll(int timeout, EventHandlerList *activeEventHandlers) override;

    virtual void update(EventHandler* handler) override;

    virtual void remove(EventHandler *handler) override;
private:
    void fillActiveEvHandler(int num, EventHandlerList * activeEvHandlers);

    void updateHandler(int opt, EventHandler* handler);
    
    static constexpr int Max_Events_Size = 100;
    int epfd_;
    std::vector<struct epoll_event> events_;
    std::map<int,EventHandler*> evHandlers_;
};

}
