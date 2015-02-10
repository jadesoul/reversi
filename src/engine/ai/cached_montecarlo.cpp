/**
 * File: cached_montecarlo.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 10:01:42.039602
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "cached_montecarlo.h"

void CachedMonteCarloAIPlayer::generate_big_book() {
	RandomAIPlayer player;//用于推演时随机下棋
	//利用蒙特卡罗思想，开始随机下棋若干次
	for_n(i, 10000) {
		Game game(player, player);//举办一场比赛
		game.start_expand_opening(openings.book);
	}
	log_warn("generated big book, size="<<openings.book.size());
}

pos_t CachedMonteCarloAIPlayer::play(Board& board) {
	while (openings.book.size()<10000000) {
		generate_big_book();
	}

	return MonteCarloAIPlayer::play(board);
}
