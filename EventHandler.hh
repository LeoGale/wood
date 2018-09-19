#include <poll.h>
#include <string.h>

namespace Wood {
class EventHandler {
public:
    EventHandler(int fd);
    ~EventHandler();

    int fd()
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

    std::string eventsStr() const;
    std::string reeventsStr() const;

    void setReevents(short reevents)
    {
        reevents_ = reevents;
    }

    static constexpr int NoneEvent;
    static constexpr int ReadEvent;
    static constexpr int WriteEvent;

  private:
    int fd_;
    int events_;
    int reevents_;
    int index_;
};
}