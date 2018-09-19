
#include <memory>
#include <vector>

namespace Wood
{
class Poller; 

class EventLoop
{
  public:
    EventLoop();
    ~EventLoop();

    void loop();
    void stop();
  private:
    bool quit_;
    bool isLooping;
    bool eventHanlding;
    std::unique_ptr<Poller> poller_;
    std::vector<EventHandler*> activeHandlers_;
};

} // namespace Wood