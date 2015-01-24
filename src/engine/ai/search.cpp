/**
 * File: search.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 10:00:50.343437
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "search.h"

pos_t LookNAIPlayer::play(Board& board) {
	log_debug(board);
	assert(board.mobility()>=2);

	pos_t pos=OpeningBookPlayer::play(board);
	if (pos!=PASS) return pos;

	//深度优先搜索
	uchar self=board.turn();
	log_status(board);

	pos_t best_pos=PASS;
	uint depth=3; //最多的搜索层数
	if (board.empty_cnt()<=10) depth=16;//当游戏快结束时，多搜索几层

	size_t total_choice=board.mobility();
	assert(total_choice>0);

	if (total_choice==1) {//如果自己只有一种走法，直接下子
		best_pos=board.first();
		assert(best_pos!=PASS);
	} else {
		uint total_searched_nodes=0;
		timer previous;
		double max_score=-1e100;

		//找出下子之后使得对方行动力最低的一步走法
		for (pos_t pos = FIRST; pos < LAST; ++pos) {
			if (board.is_active(pos)) {//自己走法

				//默认下法
				if (best_pos == PASS) {
					best_pos = pos;
				}

				if (BESIDE_GOOD_CORNER(pos)) {
					continue;
				}

				Board think1=board;

				size_t eat1=think1.play(pos);//自己吃子数
				size_t mobility1=think1.mobility();//对手行动力

				uint total_end_game=0;
				uint total_meet_depth=0;

				vector<State> history;
				State root(think1);
				double root_score=0;

				history.push_back(root);
				while (! history.empty()) {
					++total_nodes_for_each_game;
					++total_searched_nodes;

					if (total_searched_nodes%100000==0) {
						double gap=previous.elapsed();
						log_status("total="<<total_nodes_for_each_game<<" searched="<<total_searched_nodes<<" time="<<gap<<" depth="<<history.size()<<" speed="<<(0.001*total_searched_nodes/gap)<<"kn/s endgame="<<total_end_game<<" meetdepth="<<total_meet_depth);
					}

					State& current=history.back();
					if (! current.end()) {
						pos_t move=current.get_move();
						Board think=current.board;

						size_t eat=0;
						if (move==PASS) {
							think.pass();
						} else {
							eat=think.play(move);
							assert(eat>0);
						}

						if (history.size()<=depth && !think.game_over()) { // 不满足终止条件
							State next(think);
							history.push_back(next);
						} else { // 满足终止条件
							if (think.game_over()) { //游戏结束了
								++total_end_game;
								Score score(think);
								current.update_score(score.win(self));
//								current.update_score(score.sign(self));

								if (think.black_cnt()==0 || think.white_cnt()==0) log_warn(think);
							} else { //搜索深度达到了
								++total_meet_depth;
//								current.update_score(0);
								current.update_score(self==think.get_current_turn() ? think.mobility() : -think.mobility());
							}
						}
						current.next();
					} else {
						uint n=history.size();
						if (n==1) { //root出栈，处理完了
							root_score=history[0].score;
						} else if (n>1) { //搜集当前节点的评分到父节点
							history[n-2].update_score(history[n-1].score);
						}
						history.pop_back();
					}
				}

				if (root_score>max_score) {
					max_score=root_score;
					best_pos=pos;
				}
			}
		}
	}

	assert(best_pos!=PASS);
	log_info(COLOR(self)<<" LookNAIPlayer, play at "<<Move(self, best_pos));
	board.play(best_pos);

	return best_pos;
}


