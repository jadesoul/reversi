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
	assert(board.mobility()>=2);//如果自己只有一种走法，应该已经在Game中直接走掉了

	//如果空格数不多了，不要走开局
	if (board.played_cnt()<10) {
		pos_t pos=OpeningBookPlayer::play(board);
		if (pos!=PASS) return pos;
	}

	//深度优先搜索
	uchar self=board.turn();
//	log_status(board);

	uint depth=6; //最多的搜索层数
	if (board.empty_cnt()<=12) depth=16;//当游戏快结束时，多搜索几层
//	if (board.empty_cnt()<=8) depth=16;//当游戏快结束时，多搜索几层

	uint total_searched_nodes=0;
	timer previous;

	uint total_end_game=0;
	uint total_meet_depth=0;

	vector<State> history;
	history.reserve(128);
	State root(board);
	double root_score=0;
	pos_t best_pos=PASS;

	history.push_back(root);
	while (! history.empty()) {
		++total_nodes_for_each_game;
		++total_searched_nodes;

		if (total_searched_nodes%100000==0) {
			double gap=previous.elapsed();
			log_status("empty="<<board.empty_cnt()
					<<" total="<<total_nodes_for_each_game
					<<" searched="<<total_searched_nodes
					<<" time="<<gap
					<<" depth="<<history.size()
					<<" speed="<<(0.001*total_searched_nodes/gap)<<"KN/s"
					<<" endg="<<total_end_game
					<<" meetd="<<total_meet_depth
					<<" win="<< history.front().score
					);
		}

//		log_status("== Stack ==");
//		for_n(i, history.size()) {
//			log_status(history[i].board);
//			log_status("history["<<i<<"] = "<<history[i]);
//		}

		State& current=history.back();
		log_debug("State& current=history.back():"<<current);

//		getchar();

		if (! current.end()) {
			
			current.next();//每次进来先自增，再处理自增前的
			pos_t p=current.get_move_pos();
			Board think=current.board;
			assert (! think.game_over());

			size_t eat=0;
			if (p==PASS) {
				think.pass();
			} else {
				eat=think.play(p);
				assert(eat>0);
			}

//			log_status(think);
			if (history.size()<depth && !think.game_over()) { // 不满足终止条件
				State next(think, - current.beta, - current.alpha);
				history.push_back(next);
			} else { // 满足终止条件
				double win;
				if (think.game_over()) { //游戏结束了
					++total_end_game;
					Score score(think);
					win=score.win(think.turn());//*100
					current.update_score(win);
					log_debug("game over, win="<<win);
//					if (think.black_cnt()==0 or think.white_cnt()==0) {
//						log_warn(think);
//					}
				} else { //搜索深度达到了
					++total_meet_depth;
					win=think.evaluate_and_predict_win_score();
//					current.update_score(self==think.turn() ? score : -score);
					current.update_score(win);//父节点应该尽量选择子节点赢子数少或输的最多的位置下子
//					log_status("meet depth, evaluate score="<<score);
				}
			}
		} else {
			uint n=history.size();
			if (n==1) { //root出栈，处理完了
				root_score=history[0].score;
				best_pos=history[0].best;
//				log_status("current="<<current);
//				root=history[0];
			} else if (n>1) { //搜集当前节点的评分到父节点
				history[n-2].update_score(history[n-1].score);
//				if (n==2) {
//					log_status("current="<<current<<" update="<< -history[n-1].score);
//				}
//				if (current.score==8888) {
//					log_status("current="<<current);
//				}
			}
			history.pop_back();
		}
	}

	assert(best_pos!=PASS);
	log_info(COLOR(self)<<" LookNAIPlayer, play at "<<Move(self, best_pos)<<", score="<<root_score);
	board.play(best_pos, root_score);

	return best_pos;
}


