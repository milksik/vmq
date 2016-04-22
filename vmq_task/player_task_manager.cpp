#include "stdafx.h"
#include "player_task_manager.h"


player_task_manager::player_task_manager() :terminated_(false)
{

}


player_task_manager::~player_task_manager()
{
	stop();

	if (thread_ptr_.get() != nullptr)
	{
		thread_ptr_->join();
	}
}

void player_task_manager::start()
{
	thread_ptr_ = std::make_shared< std::thread >(&player_task_manager::do_work, this);
}

void player_task_manager::stop()
{
	terminated_ = true;
}


void player_task_manager::do_work()
{
	int execute_count = 0;

	while (terminated_ == false)
	{
		execute_count = execute_task();

		if (execute_count == 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void player_task_manager::create_worker( int worker_size )
{

	for (int i = 0; i < worker_size; ++i)
	{
		auto th = std::make_shared< player_worker>( this );
		th->start();

		workers_.push_back(th);
	}
}

void player_task_manager::create_player(player_key_type player_key, player_postee && f_postee)
{
	auto wsize = workers_.size();

	int worker_key = player_key % wsize;

	auto player = player_ptr(factory_->create());
	auto this_ptr = shared_from_this();

	player->set_parrent(this_ptr);
	player->set_key(player_key);

	auto iter = actor_map_.find(player_key);
	if (iter == actor_map_.end())
	{
		actor_map_.insert({ player_key, player });
	}

	//여기서 생성해서 넣는다.
	send_worker_postee(worker_key, [player_key, player, f_postee](player_worker_ptr worker)
	{
		worker->add_player(player_key, player);

		if (f_postee == nullptr)
		{
			f_postee(player, 0);
		}

	});

}


void player_task_manager::send_worker_postee(int worker_key, worker_postee && postee)
{
	auto worker = workers_[worker_key];
	worker->invoke(postee, worker);
}


void player_task_manager::send_player_postee(player_key_type key, player_postee && postee)
{
	//넣는건 직쩝 넣어버린다.
	auto iter = actor_map_.find(key);
	if (iter == actor_map_.end())
	{
		if (postee != nullptr)
		{
			postee(nullptr, -1 );
		}
	}
	else
	{
		auto& actor = iter->second;
		actor->invoke( postee, actor, 0 );
	}

}
