/**
 * File: human.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:58:21.518960
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "human.h"

uchar HumanPlayer::play(Board& b) {
	color self=b.get_current_turn();
	uint x, y;
	do {
		log_debug(b);
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" HumanPlayer, Please input point for play:");
		clog<<"x=";
		cin>>x;
		clog<<"y=";
		cin>>y;
		cout<<endl;
		log_info("(x, y)=("<<x<<", "<<y<<")");

	} while (x<8 and y<8 and b.play(x, y)==0);
	return (x<<4)+y;
}
