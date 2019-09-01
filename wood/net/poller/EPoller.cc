#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include <assert.h>
#include <strings.h>
#include <string.h>

#include "EPoller.hh"
#include "wood/net/EventHandler.hh"

namespace wood 
{
    
namespace OP
{
    static constexpr int NEW = -1;
    static constexpr int ADDED = 0;
    static constexpr int Deleted = 1;
}

EPoller::EPoller(EventLoop* loop)
:EventDemultiplexer(loop),
epfd_(epoll_create1(EPOLL_CLOEXEC)),
events_(Max_Events_Size)
{
    if(epfd_ < 0)
    {
        std::cout <<"ERROR EPOLL CREATE" << std::endl;
    }
}

EPoller::~EPoller()
{
    if(close(epfd_) < 0)
    {
        int errcode = errno;
        std::cout <<"Error closing file" << std::endl;
    }
}

void EPoller::poll(int timeout, EventHandlerList *activeEventHandlers)
{
    int numFds = epoll_wait(epfd_, &*events_.begin(), events_.size(), timeout);
    
    int errCode = errno;
    
    if(numFds > 0)
    {
        fillActiveEvHandler(numFds, activeEventHandlers);
        if(static_cast<size_t>(numFds) > events_.size())
        {
            events_.resize(events_.size() * 2);
        }
    }
    else if (numFds == 0)
    {
        std::cout <<"Nothing happened." << std::endl;
    }
    else 
    {
        //TODO How about EINTR
        std::cout <<"ERROR epoll_wait " << errCode << std::endl;
    }
}

void EPoller::update(EventHandler* handler)
{
    
    const int opt = handler->index();
    if( opt == OP::NEW)
    {
        int fd = handler->fd();
        assert(handlers_.find(fd) == std::end(handlers_));
        
        handlers_[fd] = handler;
        
        handler->setIndex(OP::ADDED);
        updateHandler(EPOLL_CTL_ADD, handler);
    }
    else 
    {
        assert(opt == OP::ADDED);
        
        if(handler->isNoneEvent())
        {
            updateHandler(EPOLL_CTL_DEL, handler);
            handler->setIndex(OP::Deleted);
        }
        else 
        {
            updateHandler(EPOLL_CTL_MOD, handler);
        }
    }
}

void EPoller::updateHandler(int opt, EventHandler* handler)
{
    struct epoll_event ev;
    bzero(&ev, sizeof ev);
    int fd = handler->fd();
    assert(fd < 0);
    ev.events = handler->events();
    ev.data.ptr = handler;
    
    if(epoll_ctl(epfd_, opt, fd, &ev) < 0)
    {
        int  aErrorCode = errno;

        std::cout <<"Erorr epoll_ctl " <<strerror(aErrorCode)
            <<" operation " << opt 
            <<" fd " << fd <<std::endl;
    }
}

void EPoller::remove(EventHandler* handler)
{
    int op = handler->index();
    int fd = handler->fd();
    
    handlers_.erase(fd);
    assert(op == OP::ADDED);
   
    updateHandler(EPOLL_CTL_DEL, handler);
    
    handler->setIndex(OP::NEW);
}

void EPoller::fillActiveEvHandler(int num, EventHandlerList * activeEvHandlers)
{
    for( int i = 0; i < num; i++)
    {
        EventHandler* evHandler = static_cast<EventHandler*>(events_[i].data.ptr);
        evHandler->setRevents(events_[i].events);
        activeEvHandlers->push_back(evHandler);
    }
}

}
