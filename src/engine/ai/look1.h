#ifndef LOOK1_H_1421891961_96
#define LOOK1_H_1421891961_96
/**
 * File: look1.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:59:21.960684
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"

//向后看一步棋的AI
class Look1AIPlayer : public AIPlayer {
//class Look1AIPlayer : public LookNAIPlayer {
public:
	uchar play(Board& b) {

//		if (b.empty_cnt()<=10) {
//			return play_dfs(b);
//		}

		uchar self=b.get_current_turn();
		log_debug(b);

		move_t opening_move = openings->lookup(b);
		if (opening_move != PASS) {
			assert(b.is_active(opening_move));
			b.play(opening_move);
			return opening_move;
		}

		uchar best_move=PASS;
		int min_mobility=INT32_MAX;
		//找出下子之后使得对方行动力最低的一步走法

		uchar mobility=b.mobility();

		for_n(x, 8) {
			for_n(y, 8) {
				if (b.is_active(x, y)) {
					Board think=b;
					uchar move=(x<<4)+y;//走法

					//默认下法
					if (best_move==PASS) {
						best_move=move;
					}

					if (mobility == 1) {//如果自己仅有1步棋可下，也不用推导了（TODO: 这个机制应该有game来支持）
						best_move=move;
						goto play1;
					}

					uchar eat=think.play(x, y);//吃子数
					int mobility1=think.mobility();//对手行动力


					if (mobility1==0) {//如果下某步棋后对手不能下子，则直接下这步棋
						best_move=move;
						goto play1;
					}

					pos_t p=POS(x, y);
					if (BESIDE_GOOD_CORNER(p)) {
						continue;
					}

					//考虑对手的潜在行动力，即所有与对手的子相邻的空格的个数（墙）
//					int p_mobility=think.potential_mobility(think.turn);
//					mobility1-=10 * p_mobility;//越多对自己越有利

//					mobility1+=0.5*eat;//综合考虑行动力与吃子数，优先选择对手行动力小，吃子数少的下法

					if (mobility1<min_mobility) {
						min_mobility=mobility1;
						best_move=move;
					}

				}
			}
		}
play1:
		assert(best_move!=PASS);
		uint x=best_move>>4, y=best_move&0x0F;
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
		b.play(x, y);
		return best_move;
	}
};


#endif /* LOOK1_H_1421891961_96 */
