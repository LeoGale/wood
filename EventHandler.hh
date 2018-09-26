#pragma once 
#include <functional>

namespace Wood {

class EventLoop;

using ReadCallback = std::function<void()>;
using WriteCallback = std::function<void()>;

class EventHandler
{
public:
	EventHandler(int fd, EventLoop* loop);
	~EventHandler() = default;

	void handleEvents();

	int fd() const 
	{
		return fd_;
	}

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

	void setReadCallback(ReadCallback cb)
	{
		readCallback_ = cb;
	}

	void enableWriting() 
	{
		events_ |= WriteEvent;
		update();
	}

	void disableWriting() {
		events_ &= ~WriteEvent;
	}

	void setWriteCallback(WriteCallback cb)
	{
		writeCallback_ = cb;
	}

	void disableAll()
	{
		events_ = NoneEvent;
		update();
	}


	int events() const {
		return events_;
	}

	void setEvents(int events)
	{
		events_ = events;
		update();
	}
	
	std::string eventsStr() const;

	int revents() const {
		return revents_;
	}

	void setRevents(int revents) {
		revents_ = revents;
	}

	std::string reventsStr() const;
	//used by poller, decrease the query complexity from O(n) to O(1)
	void setIndex(int index)
	{
		index_ = index;
	}

	int index() const {
		return index_;
	}

	void remove();

	static const int ReadEvent;
	static const int WriteEvent;
	static const int NoneEvent;

private:
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
