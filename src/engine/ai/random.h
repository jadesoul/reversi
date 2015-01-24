#ifndef RANDOM_H_1421891813_43
#define RANDOM_H_1421891813_43
/**
 * File: random.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:56:53.425544
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"

//也是很简单的AI，随机选择一个可下子的位置下子
//棋力灰常弱，可以作为Baseline
class RandomAIPlayer : public AIPlayer {
	Random random;
public:
	uchar play(Board& b) {
		uchar self=b.get_current_turn();

		log_debug(b);

		uint mobility=b.mobility();
		assert(mobility>=1);

		//uint index=random.randint(1, mobility);

		//optimize here:
		uint index=1+random.randuint(mobility-1);

		for_n(x, 8) {
			for_n(y, 8) {
				if (b.is_active(x, y)) {
					if (--index==0) {
						log_info(((self==BLACK)?"BLACK":"WHITE")<<" RandomAIPlayer, play at "<<Pos(x, y));
						b.play(x, y);
						return (x<<4)+y;
					}
				}
			}
		}
		assert(false);
		return PASS;
	}
};


#endif /* RANDOM_H_1421891813_43 */
