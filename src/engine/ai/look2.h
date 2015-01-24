#ifndef LOOK2_H_1421891987_3
#define LOOK2_H_1421891987_3
/**
 * File: look2.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:59:47.304504
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"
#include "search.h"

//向后看2步棋的AI
//class Look2AIPlayer : public AIPlayer {
class Look2AIPlayer : public LookNAIPlayer {
public:
	uchar play(Board& b) {
		uchar self=b.get_current_turn();
		log_debug(b);

		if (b.empty_cnt() <= 10) {
			return play_dfs(b);
		}

		move_t opening_move=openings->lookup(b);
		if (opening_move!=PASS) {
			assert(b.is_active(opening_move));
			b.play(opening_move);
			return opening_move;
		}


		uchar best_move=PASS;

		//两步棋后当前下子方的平均行动力
		//即两步棋后当前下子方的总行动力，除以一步棋后对手的总下法数（行动力）
		//可以理解为，一步棋后，对手的每种下法平均将给自己带来多少行动力
		float max_avg_mobility=-1;//根据最大化此值选择下法

		uint total_mobility1;//一步棋后对手的总下法数（行动力）
		uint total_mobility2;//两步棋后当前下子方的总行动力

		//找出下子之后使得对方行动力最低的一步走法
		uchar mobility=b.mobility();

		for_n(x1, 8) {
			for_n(y1, 8) {
				if (b.map[x1][y1]==ACTIVE) {
					uchar move1=(x1<<4)+y1;//自己走法

					//默认下法
					if (best_move==PASS) {
						best_move=move1;
					}

					if (mobility == 1) {//如果自己仅有1步棋可下，也不用推导了（TODO: 这个机制应该有game来支持）
						best_move=move1;
						goto play;
					}

					Board think1=b;
					uchar eat1=think1.play(x1, y1);//自己吃子数
					uchar mobility1=think1.mobility();//对手行动力

					if (mobility1==0) {//如果下某步棋后对手不能下子，则直接下这步棋
						best_move=move1;
						goto play;
					}

					pos_t p=POS(x1, y1);
					if (BESIDE_GOOD_CORNER(p)) {
						continue;
					}

					total_mobility1=mobility1;
					total_mobility2=0;

					bool oppo_has_good_move=false;
					for_n(x2, 8) {
						for_n(y2, 8) {
							if (think1.is_active(x2, y2)) {
								Board think2=think1;
								uchar move2=(x2<<4)+y2;//对手走法
								pos_t p2=POS(x2, y2);
								if (IS_GOOD_CORNER(p2)) {
									oppo_has_good_move=true;
								}

								uchar eat2=think2.play(x2, y2);//对手吃子数
								uchar mobility2=think2.mobility();//自己行动力
								total_mobility2+=mobility2;
							}
						}
					}

					if (oppo_has_good_move) {
						continue;
					}

					float avg_mobility=float(total_mobility2)/total_mobility1;
					if (avg_mobility>max_avg_mobility) {
						max_avg_mobility=avg_mobility;
						best_move=move1;
					}
				}
			}
		}
play:
		assert(best_move!=PASS);
		uint x=best_move>>4, y=best_move&0x0F;
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
		b.play(x, y);
		return best_move;
	}
};


#endif /* LOOK2_H_1421891987_3 */
