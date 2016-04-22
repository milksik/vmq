#pragma once

typedef int player_key_type;

typedef std::function<void()>								shared_postee;

class player : public actor
{
public:
	player();
	virtual ~player();

	void set_key(player_key_type key) { key_ = key; }

	void send_shared_postee(shared_postee&& postee);

private:
	player_key_type			key_;
};

typedef std::shared_ptr< player > player_ptr;


class player_factory 
{
public:
	virtual player* create()
	{
		return new player();
	}
};

