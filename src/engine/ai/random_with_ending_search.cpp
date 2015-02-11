/**
 * File: random_with_ending_search.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-11 19:15:15.824001
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "random_with_ending_search.h"

pos_t RandomWithEndingSearchAIPlayer::play(Board& board) {
	size_t mobility=board.mobility();
	assert(mobility>=2);

	if (board.played_cnt()<10) {
		pos_t pos=OpeningBookPlayer::play(board);
		if (pos!=PASS) return pos;
	}

	if (board.empty_cnt()<=10) {
		return LookNAIPlayer::play(board);
	} else {
		return random.play(board);
	}
}

