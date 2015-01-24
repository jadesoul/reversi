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

pos_t HumanPlayer::play(Board& board) {
	color self = board.turn();
	Move m;
	do {
		log_debug(board);
		log_info(COLOR(self)<<" HumanPlayer, Please input point for play:");
		clog << "move=";
		string s;
		cin >> s;
		m = Move(s);
		cout << endl;
	} while (!m.is_illegal_move() or board.play(m.pos) == 0);
	return m.pos;
}
