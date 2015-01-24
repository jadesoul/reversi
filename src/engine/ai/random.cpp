/**
 * File: random.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:56:56.090714
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "random.h"

pos_t RandomAIPlayer::play(Board& board) {
	log_debug(b);

	size_t mobility=board.mobility();
	assert(mobility>=2);

	uint index=1+random.randuint(mobility-1);

	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (board.is_active(pos)) {
			if (--index == 0) {
				log_info(COLOR(self)<<" RandomAIPlayer, play at "<<Move(board.turn(), pos));
				board.play(pos);
				return pos;
			}
		}
	}
	assert(false);
	return PASS;
}

