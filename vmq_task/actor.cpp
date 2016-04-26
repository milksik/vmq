#include "stdafx.h"
#include "actor.h"


actor::actor()
{

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

#if QUEUE_TYPE == LOCK_QUEUE
		{
			std::unique_lock<std::mutex> lock(this->queue_mutex_);

			if (task_queue_.empty())
				break;

			task = std::move(this->task_queue_.front());
			task_queue_.pop();
		}
#else
		if (task_queue_.dequeue(task) == false)
			break;

#endif

		if (task != nullptr)
		{
			try
			{
				task();
			}
			catch (const std::bad_function_call&)
			{
				std::cout << "bad function call" << std::endl;
			}
			catch (...)
			{
				std::cout << "unknown exception" << std::endl;
			}

		}
	}

	return execute_count;
}
