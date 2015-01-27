/**
 * File: book.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 17:49:22.556620
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "book.h"

pos_t OpeningBookPlayer::play(Board& board) {
//	log_status("in pos_t OpeningBookPlayer::play(Board& board)");
	uchar self=board.turn();
	pos_t pos=openings->lookup(board);
	if (pos!=PASS) {
		assert(board.is_active(pos));
		board.play(pos);
		return pos;
	}
	return PASS;
}
