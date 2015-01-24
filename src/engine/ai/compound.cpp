/**
 * File: compound.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 19:48:06.565727
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "compound.h"

pos_t CompoundAIPlayer::play(Board& board) {
	if (board.empty_cnt() <= 10) {
		return LookNAIPlayer::play(board);
	}
	return Look2AIPlayer::play(board);;
}
