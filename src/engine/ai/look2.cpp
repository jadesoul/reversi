/**
 * File: look2.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:59:49.815698
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "look2.h"

pos_t Look2AIPlayer::play(Board& board) {
	uchar self=board.turn();
	log_debug(board);
	assert(board.mobility()>=2);

	if (board.played_cnt()<10) {
		pos_t pos=OpeningBookPlayer::play(board);
		if (pos!=PASS) return pos;
	}

	pos_t best_pos=PASS;

	//两步棋后当前下子方的平均行动力
	//即两步棋后当前下子方的总行动力，除以一步棋后对手的总下法数（行动力）
	//可以理解为，一步棋后，对手的每种下法平均将给自己带来多少行动力
	float max_avg_mobility=-1;//根据最大化此值选择下法

	uint total_mobility1;//一步棋后对手的总下法数（行动力）
	uint total_mobility2;//两步棋后当前下子方的总行动力

	//找出下子之后使得对方行动力最低的一步走法
	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (board.is_active(pos)) {

			if (best_pos==PASS) {//默认下法
				best_pos=pos;
			}

			Board think1=board;
			size_t eat1=think1.play(pos);//自己吃子数
			size_t mobility1=think1.mobility();//对手行动力

			if (mobility1==0) {//如果下某步棋后对手不能下子，则直接下这步棋
				best_pos=pos;
				goto play_look2;
			}

			if (BESIDE_GOOD_CORNER(pos)) {
				continue;
			}

			total_mobility1=mobility1;
			total_mobility2=0;

			bool oppo_has_good_move=false;
			for (pos_t pos2 = FIRST; pos2 < LAST; ++pos2) {
				if (think1.is_active(pos2)) {//对手走法

					if (IS_GOOD_CORNER(pos2)) {
						oppo_has_good_move=true;
					}

					Board think2=think1;
					size_t eat2=think2.play(pos2);//对手吃子数
					size_t mobility2=think2.mobility();//自己行动力
					total_mobility2+=mobility2;
				}
			}

			if (oppo_has_good_move) {
				continue;
			}

			float avg_mobility=float(total_mobility2)/total_mobility1;
			if (avg_mobility>max_avg_mobility) {
				max_avg_mobility=avg_mobility;
				best_pos=pos;
			}
		}
	}

play_look2:
	assert(best_pos!=PASS);
	log_info(COLOR(self)<<" Look2AIPlayer, play at "<<Move(self, best_pos));
	board.play(best_pos);
	return best_pos;
}
