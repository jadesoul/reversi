/**
 * File: montecarlo.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 23:14:38.477857
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "montecarlo.h"

pos_t MonteCarloAIPlayer::play(Board& board) {
		uchar self=board.turn();
		log_debug(board);
		assert(board.mobility()>=2);

		pos_t pos=OpeningBookPlayer::play(board);
		if (pos!=PASS) return pos;

		RandomAIPlayer player;//用于推演时随机下棋
		map<pos_t, int> predict;//统计每个下子位置最终的总赢子数

		for (pos_t pos = FIRST; pos < LAST; ++pos) {
			if (board.is_active(pos)) {
				//针对每个落子点

				Board think=board;

				//如果在此处下子
				size_t eat=think.play(pos);//吃子数
				// uchar mobility=think.mobility();//对手行动力

				int summary=0;

				//利用蒙特卡罗思想，开始随机下棋若干次
				for_n(i, 100) {
					Game game(player, player, think);//举办一场比赛
					Score score=game.start();
					int diff=score.diff();//黑子数减去白子数之差

//					diff=diff>0?1:-1;//只记输赢次数，不记输赢程度

					if (self==BLACK) {//如果下子方是黑子，累加diff
						summary+=diff;
					} else {//否则，累减（黑棋赢子就是白棋输子）
						summary-=diff;
					}
				}

				predict[pos]=summary;
			}

		}

		//在所有走法中找出赢子数最大的走法
		pos_t best_pos=PASS;
		int max_diff=INT32_MIN;
		typedef map<pos_t, int> predict_t;
		for_iter(it, predict_t, predict) {
			pos_t pos=it->first;
			int diff=it->second;
			if (max_diff<diff) {
				max_diff=diff;
				best_pos=pos;
			}
		}

		assert(best_pos!=PASS);
		log_info(COLOR(self)<<" MonteCarloAIPlayer, play at "<<Move(self, best_pos));
		board.play(best_pos);
		return best_pos;
	}
