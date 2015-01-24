#ifndef OPENINGBOOK_H_1421891844_15
#define OPENINGBOOK_H_1421891844_15
/**
 * File: openingbook.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:57:24.145319
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"

class OpeningBookPlayer : public Player {
public:
	uchar play(Board& b) {

		move_t opening_move=openings->lookup(b);
		if (opening_move!=PASS) {
			assert(b.is_active(opening_move));
			b.play(opening_move);
			return opening_move;
		}

		uchar self=b.get_current_turn();
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
#endif /* OPENINGBOOK_H_1421891844_15 */
