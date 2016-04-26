// vmq_task.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#define TEST_PLAYER_MAX 50000
#define TEST_WORKER_PER_PLAYER 100

struct statistics
{
	int complte_count;
};

int main()
{
	//using namespace std::placeholders;  // for _1, _2, _3...

	std::shared_ptr< player_task_manager > mgr = std::make_shared<player_task_manager>();
	player_factory factory;


	mgr->set_player_factory(&factory);
	mgr->create_worker(100);
	mgr->start();

	while (true)
	{
		std::string str;
		std::cout << ">>";
		std::getline(std::cin, str);

		if (str == "q")
		{
			break;
		}
		else if (str == "a")
		{

			//mgr의 쓰레드에서 안전하게 처리하게 한다.
			mgr->invoke([&mgr] {
				
				for (int i = 0; i < TEST_PLAYER_MAX; i++)
				{
					auto player_key = mgr->new_player_key( i);
					mgr->create_player(player_key, nullptr );
				}
			
			});
		}
		else if (str == "w")
		{
			std::shared_ptr<statistics > complete_count_ptr = std::make_shared< statistics >();
			complete_count_ptr->complte_count = 0;

			mgr->invoke([&mgr, complete_count_ptr] 
			{

				for (int i = 0; i < TEST_PLAYER_MAX; i++)
				{

					for (int w = 0; w < TEST_WORKER_PER_PLAYER; w++)
					{
						mgr->send_player_postee( i, [i,w, complete_count_ptr](player_ptr actor, int execute_code){

							if (execute_code != 0)
							{
									

							}
							else
							{
								//std::cout << "[" << i << ", " << w << "] : run" << std::endl;
								actor->complete_count_++;
								if (actor->complete_count_ == TEST_WORKER_PER_PLAYER)
								{
									//mgr 쓰레드에서 호출된다. ( 어차피 하나의 쓰레드이기때문에 합계 데이터에 걍 완료 체크해둔다.
									actor->send_shared_postee([complete_count_ptr]
									{
										
										complete_count_ptr->complte_count++;

										if (complete_count_ptr->complte_count == TEST_PLAYER_MAX)
										{
											std::cout << "total_worker end..." << std::endl;
										}

									});
								}

								

							}

						});
					
					}
				}


				int comp = 0;
			});


		}

	}



    return 0;
}

