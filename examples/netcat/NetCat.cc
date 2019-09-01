#include <thread>
#include <assert.h>
#include <unistd.h>

#include <wood/net/EventLoop.hh>


int main()
{
        assert(wood::EventLoop::getCurerntEventLoop() == nullptr);
        wood::EventLoop loop;
        assert(wood::EventLoop::getCurerntEventLoop() == &loop);
        std::thread ex([&loop](){
                assert(wood::EventLoop::getCurerntEventLoop() == nullptr);
                sleep(2);
                for(int i= 0; i < 10; i++)
                {
                        sleep(1);
                        loop.wake();
                }
                loop.runInLoop([&loop](){
                        loop.stop();
                });
        });

        loop.loop();

        if(ex.joinable())
        {
                ex.join();
        }

}

