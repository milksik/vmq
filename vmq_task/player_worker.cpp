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
	int execute_count = 0;

	while (terminated_ == false )
	{
		//worker狼 postee贸府
		execute_count += execute_task();


		//器窃等 player狼 postee贸府

		for (auto & a : actor_map_)
		{
			execute_count += a.second->execute_task();
		}


		if (execute_count == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
	
}

