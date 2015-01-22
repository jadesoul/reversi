#ifndef MONTECARLO_H_1421853275_23
#define MONTECARLO_H_1421853275_23
/**
 * File: montecarlo.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 23:14:35.231425
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"
#include "engine/game.h"
#include "engine/openings.h"


//利用蒙特卡罗搜索算法找出赢棋概率最大的走法
//基本思想:
//	针对当前局面所有可能的下子位置，衍生出一个新的残局，并同时扮演黑白的角色随机下棋，直到终局
//	对每个残局进行一局棋的推演称作一个轮回，对每个下子位置，重复执行若干个轮回，统计输赢情况
//	计算出每个位置的输赢概率，选择胜率最高的位置落子
class MonteCarloAIPlayer : public AIPlayer {
public:
	uchar play(Board& board) {
		uchar self=board.turn;
		log_debug(board);

		move_t opening_move=openings->lookup(board);
		if (opening_move!=PASS) {
			assert(board.is_active(opening_move));
			board.play(opening_move);
			return opening_move;
		}

		RandomAIPlayer player;//用于推演时随机下棋
		map<uchar, int> predict;//统计每个下子位置最终的总赢子数

		for_n(x, 8) {
			for_n(y, 8) {
				if (board.map[x][y]==ACTIVE) {//针对每个落子点
					// log_warn("MonteCarloAIPlayer: try @ ("<<x<<", "<<y<<")");

					Board think=board;
					uchar move=(x<<4)+y;//如果在此处下子
					uchar eat=think.play(x, y);//吃子数
					// uchar mobility=think.mobility();//对手行动力

					predict[move]=0;

					//利用蒙特卡罗思想，开始随机下棋若干次
					for_n(i, 100) {
						Game game(player, player, think);//举办一场比赛
						Score score=game.start();
						int diff=score.diff();//黑子数减去白子数之差

						diff=diff>0?1:-1;//只记输赢次数，不记输赢程度

						if (self==BLACK) {//如果下子方是黑子，累加diff
							predict[move]+=diff;
						} else {//否则，累减（黑棋赢子就是白棋输子）
							predict[move]-=diff;
						}
					}
				}
			}
		}

//		list<uchar> keys=predict.keys();

		//for debug
		// log_warn("---------- predict.size()="<<predict.size());
		// for_iter(it, list<uchar>, keys) {
			// uchar move=(uchar)*it;
			// uint x=move>>4, y=move&0x0F;
			// log_warn("move="<<x<<","<<y<<" predict="<<predict[*it]);
		// }

		//在所有走法中找出赢子数最大的走法
		uchar best_move=-1;
		int max_diff=INT32_MIN;
		typedef map<uchar, int> predict_t;
		for_iter(it, predict_t, predict) {
			uchar move=it->first;
			int diff=it->second;
			if (max_diff<diff) {
				max_diff=diff;
				best_move=move;
			}
		}

		assert(best_move!=-1);
		uint x=best_move>>4, y=best_move&0x0F;
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
		board.play(x, y);
		return best_move;
	}
};

#endif /* MONTECARLO_H_1421853275_23 */
