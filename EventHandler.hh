#include <poll.h>
#include <string>

namespace Wood {
class EventHandler {
public:
    EventHandler(int fd);
    ~EventHandler();

    int fd() const
    {
        return fd_;
    }

    void handleEvent();

    void enableReading()
    {
        events_ |= ReadEvent;
    }
    void disableReading()
    {
        events_ &= ~ReadEvent;
    }

    void enableWriting()
    {
        events_ |= WriteEvent;
    }
    void disableWriting()
    {
        events_ &= ~WriteEvent;
    }
    void disableAll()
    {
        events_ = NoneEvent;
    }

    void setEvents(short events)
    {
        events_ = events;
    }

    short events() const
    {
        return events_;
    }

    short revents() const 
    {
        return revents_;
    }
    
    std::string eventsStr() const;
    std::string reventsStr() const;

    void setRevents(short revents)
    {
        revents_ = revents;
    }

    static const int NoneEvent;
    static const int ReadEvent;
    static const int WriteEvent;

  private:
    std::string eventsToString(int fd, int ev) const;
    int fd_;
    int events_;
    int revents_;
    int index_;
};
}