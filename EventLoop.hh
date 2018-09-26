
#pragma once 

#include <vector>
#include <memory>
#include <functional>
#include <mutex>

namespace Wood {

class EventDemultiplexer;
class EventHandler;
using Task = std::function<void()>;

class EventLoop {
public:
	EventLoop();
	~EventLoop();

	void loop();
	void stop();

	void wake();
	void assertInLoopThread();
	void runInLoop(Task task);
	static const EventLoop* getCurerntEventLoop();

	void updateEventHandler(EventHandler* handler);
	void removeEventHandler(EventHandler* handler);
	
private:
	bool isInEventLoopThread();
	void queueInLoop(Task task);
	void handleRead();
	void doPendingTasks();
	bool quit_;
	int wakefd_;
	const pid_t threadId_;
	std::mutex mutex_;
	std::unique_ptr<EventDemultiplexer> eventDemultiplexer_;
	std::unique_ptr<EventHandler> wakeupHandler_;
	std::vector<Task> tasks_;
};

}
