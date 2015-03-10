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
	Board 			board;	//当前棋局
	vector<pos_t> 	moves;	//当前棋局下所有可能的下法
	uint 			index;	//已经处理到了第几种下法
	double			score;	//当前节点的分数，代表在所有可能性中自己最多赢多少子
	pos_t			best;	//当孩子更新上来的分数取最大值时的下子位置，即最好的孩子位置

	int				alpha;//搜索估值下限
	int				beta;//搜索估值上限
public:
	State(const Board& board, int alpha=-64, int beta=64);

	//根据某个孩子状态的分数更新当前状态的分数
	void update_score(double child_score);

	inline bool end() { return !(index < moves.size()); }

	inline void next() { ++index; }

	//直接将index设置到end，跳过所有后续可能性
	inline void skip_all() { index=moves.size(); }

	inline pos_t get_move_pos() {
//		log_status("debug:"<<board);
//		log_status("state:"<<*this);
//		log_status("end():"<<end());
//		log_status("index:"<<index);
//		log_status("moves.size():"<<moves.size());
//		assert(1<=index && index<=moves.size());
		return moves[index-1];
	}

	ostream& dump(ostream& o=cout) const;
	friend inline ostream& operator<<(ostream& o, const State& s) { return s.dump(o); }
};

#endif /* STATE_H_1421891892_98 */
