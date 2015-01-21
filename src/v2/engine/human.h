#ifndef HUMAN_H_1421852298_01
#define HUMAN_H_1421852298_01
/**
 * File: human.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:58:18.006800
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "player.h"

class HumanPlayer : public Player {
public:
	uchar play(Board& b) {
		uchar self=b.turn;
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
};

#endif /* HUMAN_H_1421852298_01 */
