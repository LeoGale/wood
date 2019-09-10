#pragma once

#include <thread>
#include <future>
#include <iostream>
#include <type_traits>
#include <functional>


namespace wood {

template<class F, typename... Args, typename R = std::result_of<F(Args...)>::type>
class Job {
public:
	Job(F && f, Args && ... args)
	task(std::bind(std::forward<F>(f), std::forward<Args>(args)...))
	{
	}
	
	std::future<R> get_future()
	{
		return task_.get_future();
	}

	void operator()
	{
		task_();
	}
private:
	std::packaged_task<R> task_;
};

class Worker {
	using FetchJobsFunc = std::function<void(std::deque<Job>&, std::size_t)>; 
public:
	Worker(std::uint64_t id)
	:isStopped_(false),
	id_(id)
	{
		thread_ = std::thread(&Worker::run, this)
	}

	~Worker()
	{
		if(!isStopped)
		{
			stop();
		}
	}

	void stop()
	{
		isStopped_ = true;
		if(thread_.joinable)
 		{
			thread_.join();
		}
	}

	void dispatch(Job && job)
	{
		jobs_.push_back(std::forward<Job>(job));
	}

	void run()
	{
		while(!isStopped_)
		{
		if(!jobs_.empty())
		{
			for(auto & job : jobs_)
			{
				job();
			}
		}

		std::deque<Job> jobs;
		fetchJobs_(jobs, 1);

		if(jobs_.empty())
		{
			//TODO sleep a little while
			std::this_thread::yield();
		}
		else 
		{
			for(auto & job : jobs)
			{
				//Fix If it is time_consumed job, 
				job();
			}
		}
		}
	}

	void setFetchJobsHandler(FetchJobsFunc && handle)
	{
		fetchJobs_ = std::forward<FetchJobsFunc>(handle);
	}
private:
	bool isStopped_;
	std::uint64_t id_;
	std::deque<Job> jobs_;
	std::thread thread_;
	FetchJobsFunc fetchJobs_;
};

class ThreadPool {
public:
	ThreadPool::ThreadPool(std::size_t size)
	{
		for(std::size_t i = 0; i < size; i++)
		{
			workers_.emplace_back(i)
			workers_.at(i).setFetchJobsHandler(std::bind(&ThreadPool::fetchJob, this, std::placeholders::_1, std::placeholders::_2));
		}
	}

	~ThreadPool()
	{
		stop();
	}

	void stop()
	{
		for(auto & worker : workers_)
		{
			worker.stop();
		}
	}

	template<class F, typename Args... args>
	auto deliver( F f, Args... args)->decltype(std::future<std::result_of<F(Args...)>::type>)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		jobs_.emplace_back(f, args...);
		return jobs_.back().get_future();
	}

	template<class F, typename Args... args>
	auto deliver(uint64_t index, F f, Args... args)->decltype(std::future<std::result_of<F(Args...)>::type>)
	{
		if(workers_.empty())
		{
			std::runtime_error("There is no workers running in the pool");
		}
		Job job(f, args...);
		auto result = job.get_future();
		workers_.at(index%sizeof(wokers_)).dispatch(std::move(job));
	}

	void fetchJob(std::deque<Job> & jobs, size_t size)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		std::swap_ranges(jobs_.begin(), jobs_.begin()+size, jobs.begin());
		jobs_.erase(jobs_.begin(), jobs_.begin()+size());
		//jobs_.pop_front();
	}

private:
	std::deque<Worker> workers_;
	std::mutex mutex_;
	std::deque<Job> jobs_;
};
}
