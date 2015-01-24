#ifndef EASY_H_1421891030_52
#define EASY_H_1421891030_52
/**
 * File: easy.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:43:50.520303
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"

//最简单的AI，选择第一个可下子的位置下子
class EasyAIPlayer : public AIPlayer {
public:
	uchar play(Board& b) {
		uchar self=b.get_current_turn();

		log_debug(b);

		pos_t opening_move=openings->lookup(b);
		if (opening_move!=PASS) {
			assert(b.is_active(opening_move));
			b.play(opening_move);
			return opening_move;
		}

		for_n(x, 8) {
			for_n(y, 8) {
				if (b.is_active(x, y)) {
					log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
					b.play(x, y);
					return (x<<4)+y;
				}
			}
		}
		assert(false);
		return PASS;
	}
};


#endif /* EASY_H_1421891030_52 */
