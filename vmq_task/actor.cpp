#include "stdafx.h"
#include "actor.h"


actor::actor()
{
	// 일단 작업

}


actor::~actor()
{

}

int actor::execute_task( int pop_count )
{
	int execute_count = 0;
	for (int i = 0; i < pop_count; i++)
	{
		std::function<void()> task;

		{
			std::unique_lock<std::mutex> lock(this->queue_mutex_);

			if (tasks_queue_.empty())
				break;

			task = std::move(this->tasks_queue_.front());
			tasks_queue_.pop();
		}

		if (task != nullptr)
		{
			task();
			execute_count++;
		}
	}

	return execute_count;
}
