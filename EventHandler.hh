#include <poll.h>
#include <string>
#include <functional>

namespace Wood {
class EventLoop;

using ReadCallback = std::function<void()> ;
using WriteCallback = std::function<void()>;

class EventHandler {
public:
    EventHandler(int fd, EventLoop* loop);
    ~EventHandler();

    int fd() const
    {
        return fd_;
    }

    void handleEvent();

    void enableReading()
    {
        events_ |= ReadEvent;
        update();
    }
    void disableReading()
    {
        events_ &= ~ReadEvent;
        update();
    }

    void enableWriting()
    {
        events_ |= WriteEvent;
        update();
    }
    void disableWriting()
    {
        events_ &= ~WriteEvent;
        update();
    }
    void disableAll()
    {
        events_ = NoneEvent;
        update();
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
    void setReadCallback(WriteCallback callback)
    {
        readCallback_ = std::move(callback);
    }

    void setWriteCallback(ReadCallback callback)
    {
        writeCallback_ = std::move(callback);
    }

    void remove();

    static const int NoneEvent;
    static const int ReadEvent;
    static const int WriteEvent;

  private:
    std::string eventsToString(int fd, int ev) const;
    void update();

    int fd_;
    int events_;
    int revents_;
    int index_;
    EventLoop* loop_;
    ReadCallback readCallback_;
    WriteCallback writeCallback_;
};

}