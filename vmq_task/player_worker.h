#pragma once

class player_task_manager;

class player_worker : public actor
{

public:
	player_worker(player_task_manager* task_mgr) :task_manager_(task_mgr), terminated_(false), worker_count_per_sleep_(10), pop_postee_per_actor_{ 100 } {}
	virtual ~player_worker();

	void start();
	void stop();

	void add_player( player_key_type key, player_ptr actor );

private:
	void do_work();

	volatile bool										terminated_;
	player_task_manager*								task_manager_;
	std::shared_ptr< std::thread >						thread_ptr_;

	std::unordered_map< player_key_type, actor_ptr >	actor_map_;
	
	int													worker_count_per_sleep_;
	int													pop_postee_per_actor_;			//¾×ÅÍ´ç ÇÑ¹ø¿¡ »Ì´Â Postee°¹¼ö
};
typedef std::shared_ptr<  player_worker > player_worker_ptr;

