#ifndef AI_H_1372368194_48
#define AI_H_1372368194_48
/**
 * File: ai.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-28 05:23:14.481000
 * Written In: Peking University, beijing, China
 */

#include "player.h"
#include "game.h"

//所有AI的基类
//class AIPlayer : public Player {};

//optimized here:
typedef Player AIPlayer;

//最简单的AI，选择第一个可下子的位置下子
class EasyAIPlayer : public AIPlayer {
public:
	uchar play(Board& b) {
		uchar self=b.turn;
		
		log_debug(b);
		
		for_n(x, 8) {
			for_n(y, 8) {
				if (b.map[x][y]==ACTIVE) {
					log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
					b.play(x, y);
					return (x<<4)+y;
				}
			}
		}
		assert(false);
		return 0;
	}
};

//也是很简单的AI，随机选择一个可下子的位置下子
//棋力灰常弱，可以作为Baseline
class RandomAIPlayer : public AIPlayer {
	Random random;
public:
	uchar play(Board& b) {
		uchar self=b.turn;
		
		log_debug(b);
		
		int mobility=b.mobility();
		assert(mobility>=1);

		//uint index=random.randint(1, mobility);

		//optimize here:
		uint index=1+random.randuint(mobility);

		for_n(x, 8) {
			for_n(y, 8) {
				if (b.map[x][y]==ACTIVE) {
					if (--index==0) {
						log_info(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
						b.play(x, y);
						return (x<<4)+y;
					}
				}
			}
		}
		assert(false);
		return 0;
	}
};

//向后看一步棋的AI
class Look1AIPlayer : public AIPlayer {
public:
	uchar play(Board& b) {
		uchar self=b.turn;
		log_debug(b);
		
		uchar best_move=-1;
		uchar min_mobility=-1;
		//找出下子之后使得对方行动力最低的一步走法
		
		for_n(x, 8) {
			for_n(y, 8) {
				if (b.map[x][y]==ACTIVE) {
					Board think=b;
					uchar move=(x<<4)+y;//走法
					uchar eat=think.play(x, y);//吃子数
					uchar mobility=think.mobility();//对手行动力
					
					// mobility+=eat;//综合考虑行动力与吃子数，优先选择行动力小，吃子数少的下法
					
					if (mobility<min_mobility) {
						min_mobility=mobility;
						best_move=move;
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

//向后看2步棋的AI
class Look2AIPlayer : public AIPlayer {
public:
	uchar play(Board& b) {
		uchar self=b.turn;
		log_debug(b);
		
		uchar best_move=-1;
		
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
					if (best_move==-1) {
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

					if (IS_BAD_MOVE(move1)) {//TODO: BUG
						continue;
					}

					total_mobility1=mobility1;
					total_mobility2=0;
					
					for_n(x2, 8) {
						for_n(y2, 8) {
							if (think1.map[x2][y2]==ACTIVE) {
								Board think2=think1;
								uchar move2=(x2<<4)+y2;//对手走法
								uchar eat2=think2.play(x2, y2);//对手吃子数
								uchar mobility2=think2.mobility();//自己行动力
								total_mobility2+=mobility2;	
							}
						}
					}
					
					float avg_mobility=float(total_mobility2)/total_mobility1;
//					if (avg_mobility>max_avg_mobility and NOT_BAD_MOVE(move1)) {//TODO: 这里有BUG
					if (avg_mobility>max_avg_mobility) {
						max_avg_mobility=avg_mobility;
						best_move=move1;
					}
				}
			}
		}
play:
		assert(best_move!=-1);
		uint x=best_move>>4, y=best_move&0x0F;
		log_warn(((self==BLACK)?"BLACK":"WHITE")<<" AIPlayer, play at ("<<x<<", "<<y<<")");
		b.play(x, y);
		return best_move;
	}
};

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

class State {
public:
	Board 		board;	//当前棋局
	vector<uchar> 	moves;	//当前棋局下所有可能的下法
	uchar 		index;	//已经处理到了第几种下法
	double		score;	//评估函数：当前节点的分数
	
public:
	State(const Board& board) {
		this->board=board;
		uchar mobility=board.mobility();
		if (mobility==0) {
			moves.push_back(PASS);
		} else {
			for_n(x1, 8) {
				for_n(y1, 8) {
					if (board.map[x1][y1]==ACTIVE) {
						uchar move=(x1<<4)+y1;//自己走法
						moves.push_back(move);
					}
				}
			}
			std::random_shuffle(moves.begin(), moves.end());
			assert(moves.size()==mobility);
		}
		index=0;
		score=0;
	}
	
	void update_score(double child_score) {//根据某个孩子状态的分数更新当前状态的分数
		// if (child_score>score) score=child_score;
		// if (child_score>score) score=(score+child_score)/2;
		// score=(score+child_score)/2;
		score+=child_score/moves.size();
	}
	
	bool end() {
		return !(index<moves.size());
	}
	
	void next() {
		++index;
	}
	
	uchar get_move() {
		assert(!end());
		return moves[index];
	}
};

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
	};
	
	uchar play(Board& b) {
		return play_dfs(b);
		// return play_bfs(b);
	}
	
	uchar play_dfs(Board& b) {//深度优先搜索
		uchar self=b.turn;
//		log_status(b);
		
		uchar best_move=-1;
		uchar depth=3; //最多的搜索层数
		if (b.empty_cnt()<10) depth=16;
		
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
					if (b.map[x1][y1]==ACTIVE) {
						Board think1=b;
						uchar move1=(x1<<4)+y1;//自己走法
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
										if (think.black_cnt()==0 || think.white_cnt()==0) log_warn(think);
									} else { //搜索深度达到了
										++total_meet_depth;
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
		
		assert(best_move!=-1);
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
									total_mobility_by_search+=score.win(self)*10;
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

#endif /* AI_H_1372368194_48 */
