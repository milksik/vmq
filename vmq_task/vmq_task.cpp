// vmq_task.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


int main()
{
	using namespace std::placeholders;  // for _1, _2, _3...

	std::shared_ptr< player_task_manager > mgr = std::make_shared<player_task_manager>();
	player_factory factory;

	mgr->set_player_factory(&factory);
	mgr->create_worker(10);
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

			//mgr�� �����忡�� �����ϰ� ó���ϰ� �Ѵ�.
			mgr->invoke([&mgr] {
				
				for (int i = 0; i < 10000; i++)
				{
					auto player_key = mgr->new_player_key( i);
					mgr->create_player( player_key, nullptr );
				}
			
			});
		}
		else if (str == "w")
		{

			mgr->invoke([&mgr] {

				for (int i = 0; i < 10000; i++)
				{

					for (int w = 0; w < 1000; w++)
					{
						mgr->send_player_postee( i, [](actor_ptr actor, int execute_code){

							int ibreak = 0;


						});
					
					}
				}

			});


		}

	}



    return 0;
}

