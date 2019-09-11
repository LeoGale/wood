#pragma once

#include <thread>
#include <future>
#include <iostream>
#include <type_traits>
#include <functional>
#include <deque>
#include <iterator>

namespace wood 
{

using Task = std::function<void()>;

class Worker 
{
public:
    Worker(std::uint64_t id)
    :isStopped_(false),
    id_(id)
    {
        thread_ = std::thread(&Worker::run, this);//FIXME safe ?
    }

    ~Worker()
    {
        if(!isStopped_)
        {
            //FIXME callonce
            stop();
        }
    }

    void stop()
    {
        isStopped_ = true;
        if(thread_.joinable())
        {
            thread_.join();
        }
    }

    void assign(Task && task)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push_back(std::forward<Task>(task));
        cond_.notify_one();
    }

    void run()
    {
        std::cout <<"worker id " << (int)id_ << " in thread " << thread_.get_id() << std::endl;
        while(!isStopped_)
        {
            std::deque<Task> tasks;
            {
                std::unique_lock<std::mutex> lock(mutex_);
            
                cond_.wait(lock, [this](){return !tasks_.empty();});
                
                tasks = std::move(tasks_);
            }
               
            std::cout <<"worker id " << (int)id_ << " in thread " << thread_.get_id() <<" task size:" << tasks.size()<< std::endl;

            for(auto & task : tasks)
            {
                task();
            }
        }
    }

private:
    std::mutex mutex_;
    std::condition_variable cond_;
    bool isStopped_;
    std::uint64_t id_;
    std::deque<Task> tasks_;
    std::thread thread_;
};

class ThreadPool {
public:
    ThreadPool(std::size_t size)
    {
        for(std::size_t i = 1; i <= size; i++)
        {
            workers_.emplace_back(i);
        }
    }

    ~ThreadPool()
    {
        //FIXME check flag
        stop();
    }

    void stop()
    {
        for(auto & worker : workers_)
        {
            worker.stop();
        }
    }

    template<class F, typename ... Args>
    std::future<typename std::result_of<F(Args...)>::type> deliver( F && f, Args && ... args)
    {
        return do_deliver(getWorkerIndex(), std::forward<F>(f), std::forward<Args>(args)...); 
    }

    template<class F, typename ... Args>
    std::future<typename std::result_of<F(Args...)>::type> deliver(uint64_t index, F && f, Args &&... args)
    {
        return do_deliver(index, std::forward<F>(f), std::forward<Args>(args)...);
    }

private:
    template <class F, typename ... Args>
    std::future<typename std::result_of<F(Args...)>::type> do_deliver(uint64_t index, F && f, Args && ... args)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if(workers_.empty())
        {
            std::runtime_error("There is no workers running in the pool");
        }
        using ReturnType = typename std::result_of<F(Args...)>::type;
        
        using Packeged_Task = std::packaged_task<ReturnType()>;
        
        std::shared_ptr<Packeged_Task> task= std::make_shared<Packeged_Task>(std::bind(f, args...));

        workers_.at(index%workers_.size()).assign([task](){(*task)();});
        return task->get_future();
    }
    
    uint64_t getWorkerIndex()
    {
        //FIXME add more strategies
        static uint64_t tasksNum = 1;
        tasksNum++;
    }

    std::deque<Worker> workers_;
    std::mutex mutex_;
};
}
