// altered https://github.com/progschj/ThreadPool

#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <future>
#include <memory>
#include <condition_variable>
#include <functional>
#include <stdexcept>

class thread_pool {
public:
	// launch workers
	thread_pool(size_t threads) 
		: stop(false)
	{
		for (size_t i = 0; i < threads; i++)
		{
			workers.emplace_back(
				[this]
				{
					while (true)
					{
						std::function<void()> task;

						{
							std::unique_lock<std::mutex> lock(this->queue_mutex);
							this->condition.wait(lock,
								[this] {return this->stop || !this->tasks.empty(); });
							if (this->stop && this->tasks.empty())
								return;
							task = std::move(this->tasks.front());
							this->tasks.pop();
						}

						task();
					}
				}
			);
		}
	}

	// add new work item to the pool
	template<class F, class... Args>
	auto enqueue(F&& f, Args&&... args)
		-> std::future<typename std::result_of<F(Args...)>::type>
	{
		using return_type = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared<std::packaged_task<return_type()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
			);

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);

			// don't allow enqueueing after stopping the pool
			if (stop)
				throw std::runtime_error("enqueue on stopped thread_pool");

			tasks.emplace([task]() { (*task)(); });
		}
		condition.notify_one();
		return res;
	}

	~thread_pool()
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			stop = true;
		}

		condition.notify_all();
		for (std::thread& worker : workers)
			worker.join();
	}

private:
	// the working threads
	std::vector<std::thread> workers;
	// the queue of tasks
	std::queue<std::function<void()>> tasks;
	
	// synchronization
	std::mutex queue_mutex;
	std::condition_variable condition;
	bool stop;

};

#endif // !THREAD_POOL_H
