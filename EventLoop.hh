
#include <memory>
#include <vector>
#include <functional>

namespace Wood
{

using Task = std::function<void()>;

class EventDemultiplexer; 
class EventHandler;

class EventLoop
{
  public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();
    void updateEventHandler(EventHandler* eventHandler);
    void removeEventHandler(EventHandler* eventHandler);
    void runInLoop(Task task);
    void assertInLoopThread();

  private:
    void wake();
    void queueInLoop(Task task);
    void handleRead();
    void runPendingTasks();
    bool isInLoopThread() const;
    void abortNotInLoopThread();
    bool quit_;
    bool isLooping_;
    bool eventHanlding_;
    int wakefd_;
    const pid_t threadId_;
    std::unique_ptr<EventHandler> wakeHandler_;
    std::unique_ptr<EventDemultiplexer> eventDemultiplexer_;
    std::vector<Task> tasks;
    std::vector<EventHandler*> activeHandlers_;

};

} // namespace Wood