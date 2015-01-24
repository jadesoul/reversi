/**
 * File: easy.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:43:54.054094
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "easy.h"

pos_t EasyAIPlayer::play(Board& board) {
	uchar self=board.turn();

	log_debug(board);

	pos_t pos=OpenBookPlayer::play(board);
	if (pos!=PASS) return pos;

	pos=board.first();
	log_info(COLOR(self)<<" EasyAIPlayer, play at "<<Move(self, pos));
	board.play(pos);
	return pos;
}
