#ifndef SEARCH_H_1421892047_31
#define SEARCH_H_1421892047_31
/**
 * File: search.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 10:00:47.310427
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"



//向后看 N 步棋的 AI
class LookNAIPlayer : public AIPlayer {
private:
	uint total_nodes_for_each_game;

public:
	LookNAIPlayer() {
		reset();
	}

	void reset() {
		total_nodes_for_each_game=0;
	}

	uchar play(Board& b) {

		move_t opening_move=openings->lookup(b);
		if (opening_move!=PASS) {
			assert(b.is_active(opening_move));
			b.play(opening_move);
			return opening_move;
		}

		return play_dfs(b);
//		return play_bfs(b);
	}

	uchar play_dfs(Board& b) {//深度优先搜索
		uchar self=b.get_current_turn();
		log_status(b);

		uchar best_move=PASS;
		uchar depth=3; //最多的搜索层数
		if (b.empty_cnt()<=10) depth=16;//当游戏快结束时，多搜索几层

		uchar total_choice=b.mobility();
		assert(total_choice>0);//在Game中会pass（TODO: 将处理pass的逻辑放到play函数中）
		if (total_choice==1) {//如果自己只有一种走法，直接下子
			best_move=b.get_first_move();
			assert(best_move!=PASS);
		} else {
			uint total_searched_nodes=0;
			timer previous;
			double max_score=-1e100;

			//找出下子之后使得对方行动力最低的一步走法
			for_n(x1, 8) {
				for_n(y1, 8) {
					if (b.is_active(x1, y1)) {
						uchar move1=(x1<<4)+y1;//自己走法

						//默认下法
						if (best_move == PASS) {
							best_move = move1;
						}

						if (IS_BAD_POS2(x1, y1)) {
							continue;
						}

						Board think1=b;

						uchar eat1=think1.play(x1, y1);//自己吃子数
						uchar mobility1=think1.mobility();//对手行动力

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
								uchar move=current.get_move();
								Board think=current.board;

								uchar eat=0;
								if (move==PASS) {
									think.pass();
								} else {
									uchar eat=think.play(move);
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
//										current.update_score(score.sign(self));

										if (think.black_cnt()==0 || think.white_cnt()==0) log_warn(think);
									} else { //搜索深度达到了
										++total_meet_depth;
//										current.update_score(0);
										current.update_score(self==think.turn ? think.mobility() : -think.mobility());
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
							best_move=move1;
						}
					}
				}
			}
		}

		assert(best_move!=PASS);
		uint x=best_move>>4, y=best_move&0x0F;
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
		b.play(x, y);

		return best_move;
	}

	uchar play_bfs(Board& b) {//广度优先搜索
		uchar self=b.turn;
		log_debug(b);
		log_status(b);

		uchar best_move=-1;
		uint max_nodes=100000;//最多搜索节点数，用来控制搜索层数
		uchar total_choice=b.mobility();
		assert(total_choice!=0);//在Game中会pass
		uint max_nodes_for_each_choice=max_nodes/total_choice;

		uint total_searched_notes=0;
		timer previous;

		//两步棋后当前下子方的平均行动力
		//即两步棋后当前下子方的总行动力，除以一步棋后对手的总下法数（行动力）
		//可以理解为，一步棋后，对手的每种下法平均将给自己带来多少行动力
		float max_avg_mobility=-1;//根据最大化此值选择下法

		uint total_mobility1;//一步棋后对手的总下法数（行动力）
		uint total_mobility2;//两步棋后当前下子方的总行动力
		double max_avg_mobility_by_search=-1.0e100;

		//找出下子之后使得对方行动力最低的一步走法
		for_n(x1, 8) {
			for_n(y1, 8) {
				if (b.map[x1][y1]==ACTIVE) {
					Board think1=b;
					uchar move1=(x1<<4)+y1;//自己走法
					uchar eat1=think1.play(x1, y1);//自己吃子数
					uchar mobility1=think1.mobility();//对手行动力

					if (mobility1==0) {//如果下某步棋后对手不能下子，则直接下这步棋
						//TODO
						// goto play;
					}
					// total_mobility1=mobility1;
					// total_mobility2=0;

					int total_mobility_by_search;//通过搜索得到的所有可能的N层局面上的行动力之和，或负的对手行动力之和
					uint searched_nodes_for_each_choice=0;
					uint total_end_game=0;
					uint total_meet_limit=0;

					//构造堆栈，深度优先或者广度优先，向下展开节点时遇到最后一步棋棋盘结束则停止
					//从当前下子点开始，搜索N层
					//将 每层自己的行动力（轮到自己下子）或 对手的行动力（轮到对手下子）的负值 累加，得到当前下子点的估值

					queue<State> history;
					State root(think1);

					history.push(root);
					while (! history.empty()) {
						++total_nodes_for_each_game;
						++total_searched_notes;
						++searched_nodes_for_each_choice;

						State& current=history.front();

						if ((b.empty_cnt()>32 && searched_nodes_for_each_choice%100000==0) || (b.empty_cnt()<=32 && searched_nodes_for_each_choice%10000==0)) {
							double gap=previous.elapsed();
							previous.restart();

							log_status("total="<<total_nodes_for_each_game<<" searched="<<total_searched_notes<<" choice="<<searched_nodes_for_each_choice<<" queue="<<history.size()<<" meetlimit="<<total_meet_limit<<" speed="<<(1000/gap)<<"kn/s endgame="<<total_end_game);
						}

						if (! current.end()) {
							uchar move=current.get_move();
							Board think=current.board;

							if (move==PASS) {
								think.pass();
								if (think.game_over()) {
									++total_end_game;
									current.next();
									continue;
								} else {
									// uchar mobility=think.mobility();//对手行动力
								}
							} else {
								uchar eat=think.play(move);
								// uchar mobility=think.mobility();//对手行动力
							}

							// 不满足终止条件
							// if (history.size()<N && !think.game_over()) {
							if (searched_nodes_for_each_choice<max_nodes_for_each_choice && !think.game_over()) {
								State next(think);
								history.push(next);
							} else { // 满足终止条件
								if (think.game_over()) {
									++total_end_game;
									Score score(think);
									total_mobility_by_search+=score.win(self);
								} else {
									++total_meet_limit;
									if (self==think.turn)
										total_mobility_by_search+=think.mobility();
									else
										total_mobility_by_search-=think.mobility();
								}
							}
							current.next();
						} else {
							history.pop();
						}
	 				}

					double avg_mobility_by_search=double(total_mobility_by_search)/(total_end_game+total_meet_limit+1);
					if (avg_mobility_by_search>max_avg_mobility_by_search) {
						max_avg_mobility_by_search=avg_mobility_by_search;
						best_move=move1;
					}
				}
			}
		}

		assert(best_move!=-1);
		uint x=best_move>>4, y=best_move&0x0F;
		log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
		b.play(x, y);


		return best_move;
	}

};

#endif /* SEARCH_H_1421892047_31 */
