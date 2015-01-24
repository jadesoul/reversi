/**
 * File: look1.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:59:25.651180
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "look1.h"

pos_t Look1AIPlayer::play(Board& board) {
	log_debug(b);

	pos_t pos=OpeningBookPlayer::play(board);
	if (pos!=PASS) return pos;

	pos_t best_pos=PASS;
	int min_mobility=INT32_MAX;

	//找出下子之后使得对方行动力最低的一步走法
	size_t mobility=board.mobility();
	assert(mobility>=2);

	for (int pos = FIRST; pos < LAST; ++pos) {
		if (board.is_active(pos)) {
			Board think=board;

			if (best_pos == PASS) {//存储默认下法
				best_pos = pos;
			}

			size_t eat = think.play(pos); //吃子数
			size_t mobility1 = think.mobility(); //对手行动力

			if (mobility1 == 0) { //如果下某步棋后对手不能下子，则直接下这步棋
				best_pos = pos;
				goto play_look1;
			}

			if (BESIDE_GOOD_CORNER(pos)) {//挨着角的三个子不好
				continue;
			}

//			考虑对手的潜在行动力，即所有与对手的子相邻的空格的个数（墙）
//			int p_mobility=think.potential_mobility(think.turn);
//			mobility1-=10 * p_mobility;//越多对自己越有利
//
//			mobility1+=0.5*eat;//综合考虑行动力与吃子数，优先选择对手行动力小，吃子数少的下法

			if (mobility1 < min_mobility) {
				min_mobility = mobility1;
				best_pos = pos;
			}

		}
	}
play_look1:
	assert(best_pos!=PASS);
	log_info(COLOR(self)<<" Look1AIPlayer, play at "<<Move(board.turn(), best_pos));
	board.play(best_pos);
	return best_pos;
}

