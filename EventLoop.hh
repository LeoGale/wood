
#include <memory>
#include <vector>
#include <functional>

namespace Wood
{

using Task = std::function<void()>;

class Poller; 
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
  private:
    void weak();
    void queueInLoop(Task task);
    void handleRead();
    void runPendingTasks();
    bool quit_;
    bool isLooping;
    bool eventHanlding;
    int wakefd_;
    std::unique_ptr<EventHandler> eventHandler_;
    std::unique_ptr<Poller> poller_;
    std::vector<Task> tasks;
    std::vector<EventHandler*> activeHandlers_;

};

} // namespace Wood