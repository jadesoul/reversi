#ifndef STATE_H_1421891892_98
#define STATE_H_1421891892_98
/**
 * File: state.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:58:12.976126
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"

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

	inline void update_score(double child_score) {//根据某个孩子状态的分数更新当前状态的分数
		if (child_score>score) score=child_score;
//		if (child_score>score) score=(score+child_score)/2;
//		if (child_score>score) score=(2*score+child_score)/3;
//		if (child_score>score) score=(score+2*child_score)/3;

		// score=(score+child_score)/2;

//		score+=child_score/moves.size();

//		score+=child_score;
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

#endif /* STATE_H_1421891892_98 */
