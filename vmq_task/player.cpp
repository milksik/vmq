#include "stdafx.h"
#include "player.h"


player::player() :key_(player_key_type())
{
}


player::~player()
{
}


void player::send_shared_postee(shared_postee && postee)
{
	if (parent_actor_ptr != nullptr)
	{
		parent_actor_ptr->invoke(postee);
	}
}


