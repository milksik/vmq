#include "stdafx.h"
#include "player_worker.h"
#include "player_task_manager.h"

player_worker::~player_worker()
{
	stop();

	if (thread_ptr_.get() != nullptr)
	{
		thread_ptr_->join();
	}
}

void player_worker::start()
{
	thread_ptr_ = std::make_shared< std::thread >(&player_worker::do_work, this);
}

void player_worker::stop()
{
	terminated_ = true;
}


void player_worker::add_player(player_key_type key,  player_ptr actor )
{
	
	actor_map_.insert( { key, actor });


}



void player_worker::do_work()
{
	

	while (terminated_ == false )
	{
		int execute_count = 0;
		//worker의 postee처리
		execute_count += execute_task();

		int loop_index = 0;

		if (actor_map_.size() > 0)
		{
			for (auto & a : actor_map_)
			{
				execute_count += a.second->execute_task(pop_postee_per_actor_);;

				//무한 루프 돌리면 cpu 100먹으니. 비작업이 worker_count_per_sleep_ 싸이즈 단위 이면 한번 스립걸어주고,
				if (loop_index != 0 && loop_index % worker_count_per_sleep_ == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}

				if (execute_count % worker_count_per_sleep_ == 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
				}

				if (execute_count == 0)
					loop_index++;
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}



	}
	
}

