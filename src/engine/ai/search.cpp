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

	pos_t pos=OpeningBookPlayer::play(board);
	if (pos!=PASS) return pos;

	//深度优先搜索
	uchar self=board.turn();
//	log_status(board);

	uint depth=4; //最多的搜索层数
	if (board.empty_cnt()<=10) depth=16;//当游戏快结束时，多搜索几层

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
					<<" speed="<<(0.001*total_searched_nodes/gap)<<"kn/s"
					<<" endgame="<<total_end_game
					<<" meetdepth="<<total_meet_depth);
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
//			log_debug("now current.next():"<<current);
			current.next();//每次进来先自增，再处理自增前的
//			log_debug("after current.next():"<<current);

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
//				assert(!think.game_over());
				State next(think);
				history.push_back(next);
			} else { // 满足终止条件
				if (think.game_over()) { //游戏结束了
					++total_end_game;
					Score score(think);
					int win=score.win(think.turn());
					current.update_score(- win);
					log_debug("game over, win="<<win);
					if (think.black_cnt()==0 or think.white_cnt()==0) log_warn(think);
				} else { //搜索深度达到了
					++total_meet_depth;
					double score=think.evaluate_and_predict_win_score();
//					current.update_score(self==think.turn() ? score : -score);
					current.update_score(-score);//父节点应该尽量选择子节点赢子数少或输的最多的位置下子
//					log_status("meet depth, evaluate score="<<score);
				}
			}
		} else {
			uint n=history.size();
			if (n==1) { //root出栈，处理完了
				root_score=history[0].score;
				best_pos=history[0].best;

//				if (BESIDE_GOOD_CORNER(best_pos)) {
//					if (history[0].moves.size()<=1) continue;
//					for_n(k, history[0].moves.size()) {
//						if (history[0].moves[k]!=best_pos) {
//							best_pos=history[0].moves[k];
//							break;
//						}
//					}
//				}

			} else if (n>1) { //搜集当前节点的评分到父节点
				history[n-2].update_score(- history[n-1].score);
			}
			history.pop_back();
		}
	}

	/*
	for (pos_t pos = FIRST; pos < LAST; ++pos) {
		if (board.is_active(pos)) {//自己走法

			//第一步棋作为默认下法
			if (best_pos == PASS) {
				best_pos = pos;
			}
//
//			if (BESIDE_GOOD_CORNER(pos)) {
//				continue;
//			}

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
					log_status("empty="<<board.empty_cnt()
							<<" total="<<total_nodes_for_each_game
							<<" searched="<<total_searched_nodes
							<<" time="<<gap
							<<" depth="<<history.size()
							<<" speed="<<(0.001*total_searched_nodes/gap)<<"kn/s"
							<<" endgame="<<total_end_game
							<<" meetdepth="<<total_meet_depth);
				}

				State& current=history.back();
				if (! current.end()) {
					pos_t p=current.get_move_pos();
					Board think=current.board;

					size_t eat=0;
					if (p==PASS) {
						think.pass();
					} else {
						eat=think.play(p);
						assert(eat>0);
					}

					if (history.size()<depth && !think.game_over()) { // 不满足终止条件
						State next(think);
						history.push_back(next);
					} else { // 满足终止条件
						if (think.game_over()) { //游戏结束了
							++total_end_game;
							Score score(think);
							current.update_score(score.win(self));
//								current.update_score(score.sign(self));
							if (think.black_cnt()==0 or think.white_cnt()==0) log_warn(think);
						} else { //搜索深度达到了
							++total_meet_depth;
//								current.update_score(0);
							double score=think.evaluate_and_predict_win_score();
							current.update_score(self==think.turn() ? score : -score);
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

				log_status("== Stack ==");
				for_n(i, history.size()) {
					log_status(history[i].board);
					log_status("history["<<i<<"] = "<<history[i]);
				}
				getchar();
			}

			if (root_score>max_score) {
				max_score=root_score;
				best_pos=pos;
			}
		}
	}
	*/

	assert(best_pos!=PASS);
	log_status(COLOR(self)<<" LookNAIPlayer, play at "<<Move(self, best_pos)<<", score="<<root_score);
	board.play(best_pos);

	return best_pos;
}


