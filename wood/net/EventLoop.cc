#include <sys/eventfd.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>

#include "EventLoop.hh"
#include "EventDemultiplexer.hh"
#include "wood/base/CurrentThread.hh"
#include "EventHandler.hh"

namespace wood {

namespace {

	__thread const EventLoop* t_eventLoopInThread = nullptr;

	constexpr int PollTimeout = 10000;

	int createEventFd()
	{
		int fd = ::eventfd(0, EFD_CLOEXEC  | EFD_NONBLOCK );
		if(fd < 0)
		{
			std::cout << "createEventfd failed exit" << std::endl;
			abort();
		}
	}
}

const EventLoop* EventLoop::getCurerntEventLoop()
{
	return t_eventLoopInThread;
}

EventLoop::EventLoop()
:quit_(true),
wakefd_(createEventFd()),
threadId_(CurrentThread::tid()),
eventDemultiplexer_(EventDemultiplexer::createDefaultPoller(this)),
wakeupHandler_(new EventHandler(wakefd_, this))
{
	wakeupHandler_->setReadCallback(std::bind(&EventLoop::handleRead, this));
	wakeupHandler_->enableReading();
	t_eventLoopInThread = this;
}

EventLoop::~EventLoop() 
{
	wakeupHandler_->disableAll();
	wakeupHandler_->remove();
	::close(wakefd_);
	t_eventLoopInThread = nullptr;
}

void EventLoop::stop()
{
	quit_ = true;
}

void EventLoop::loop() 
{
	assertInLoopThread();
	quit_ = false;
	while(!quit_)
	{
		EventHandlerList activeHandlers;

		eventDemultiplexer_->poll(PollTimeout, &activeHandlers);

		for(auto & handler : activeHandlers)
		{
			handler->handleEvents();
		}

		doPendingTasks();
	}
}


void EventLoop::doPendingTasks()
{
	std::vector<Task> tasks;
	{
		std::lock_guard<std::mutex> guard(mutex_);
		tasks = std::move(tasks_);
	}

	for(auto& task : tasks)
	{
		task();
	}
}

void EventLoop::wake() 
{
	std::cout <<"EventLoop::wake" << std::endl;
	int64_t data = 1;
	int nWrite = ::write(wakefd_, &data, sizeof(data));
	if(nWrite != sizeof(data))
	{
		std::cout <<"write " << nWrite << std::endl;
	}
}

void EventLoop::handleRead() {
	std::cout <<"EventLoop::handleRead" << std::endl;
	int64_t data = 1;
	int nRead = ::read(wakefd_, &data, sizeof(data));
	if(nRead != sizeof(data))
	{
		std::cout <<"Read " << nRead << std::endl;
	}
	std::cout <<"EventLoop::handleRead data " << (int)data << std::endl;
}

void EventLoop::runInLoop(Task task)
{
	if(isInEventLoopThread())
	{
		task();
	}
	else
	{
		queueInLoop(task);
	}
}

void EventLoop::assertInLoopThread() {
	if(isInEventLoopThread())
	{
		std::cout <<"EventLoop::assertInLoopThread running in event loop thread." << std::endl;
	}
	else
	{
		std::cout <<"EventLoop::assertInLoopThread running in diffrent thread from event loop thread." << std::endl;
	}
}

void EventLoop::queueInLoop(Task task)
{
	std::lock_guard<std::mutex> guard(mutex_);
	tasks_.push_back(task);
}

bool EventLoop::isInEventLoopThread() {
	return threadId_ == CurrentThread::tid();
}

void EventLoop::updateEventHandler(EventHandler* handler)
{
	eventDemultiplexer_->update(handler);
}

void EventLoop::removeEventHandler(EventHandler* handler)
{
	eventDemultiplexer_->remove(handler);
}

}
