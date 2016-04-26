#pragma once


#define VMQ_PLAYER_COUNT				(USHRT_MAX)
#define VMQ_FIELD_COUNT					(USHRT_MAX)
#define VMQ_SERVICE_COUNT				(USHRT_MAX)



typedef std::function<void(player_ptr, int execute_code )>	player_postee;
typedef std::function<void(player_worker_ptr)>				worker_postee;



class player_task_manager : public actor, public  std::enable_shared_from_this<actor>
{
	
public:
	player_task_manager();
	virtual ~player_task_manager();
	void start();
	void stop();
	void set_player_factory(player_factory* factory) { factory_ = factory; }
	
	player_key_type new_player_key(int n) { return n; }

	void create_worker(int worker_size );
	void create_player(player_key_type player_key, player_postee&& f_postee);
	
	void send_worker_postee(int worker_key, worker_postee&& postee);					//add, delete player 처리
	void send_player_postee(player_key_type key, player_postee&& postee);				//player queue에 전단시

private:
	void do_work();

	player_factory*														factory_;
	volatile bool														terminated_;
	std::shared_ptr< std::thread >										thread_ptr_;
	std::vector< player_worker_ptr >									workers_;
	std::unordered_map< player_key_type, player_ptr >					actor_map_;
	
};

