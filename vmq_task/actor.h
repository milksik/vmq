#pragma once

#define POP_TASK_COUNT 1



class actor
{
public:
	actor();
	virtual ~actor();

	template<class F, class... Args>
	auto invoke( F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;
	inline void set_parrent(std::shared_ptr< actor > parent) 
	{ 
		parent_actor_ptr = parent; 
	}
	int execute_task(int pop_count = POP_TASK_COUNT );

public:

	//queue를 mpsc로 바꾼다.
#if QUEUE_TYPE == LOCK_QUEUE
	std::queue< std::function<void()> >			task_queue_;
	std::mutex									queue_mutex_;
#elif QUEUE_TYPE == MPSC_QUEUE
	mpsc_queue_t<std::function<void()> >		task_queue_;
#endif

	std::shared_ptr< actor >					parent_actor_ptr;
};

typedef std::shared_ptr< actor > actor_ptr;


template<class F, class... Args>
auto actor::invoke(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type>
{
	using return_type = typename std::result_of<F(Args...)>::type;

	auto task = std::make_shared< std::packaged_task<return_type()> >
		(
			//템플릿 함수는 우측값 참조를 추론한다. f.args가 좌측값이어도 컴파일 에러가 안난다.
			//forward는 템플릿 함수에서 좌측값은 촤측값, 우측값은 우측값으로 바꾼다. ( 원래의 형태 )
			std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

	std::future<return_type> res = task->get_future();

#if QUEUE_TYPE == LOCK_QUEUE
	{
		std::unique_lock<std::mutex> lock(queue_mutex_);
		task_queue_.emplace([task]() { (*task)(); });
	}
#else

	task_queue_.emplace ( [task](){ (*task)();});

#endif
	//결과에 get해서 wait을 사용할수 있다.
	return res;
}

