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
	State(const Board& board);

	//根据某个孩子状态的分数更新当前状态的分数
	void update_score(double child_score);

	inline bool end() { return !(index<moves.size()); }

	inline void next() { ++index; }

	inline uchar get_move() { assert(!end()); return moves[index]; }
};

#endif /* STATE_H_1421891892_98 */
