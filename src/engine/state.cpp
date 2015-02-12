/**
 * File: state.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:58:20.961785
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "state.h"

State::State(const Board& board, int alpha, int beta):
	board(board),
	alpha(alpha),
	beta(beta)
{
	moves.reserve(24);
	size_t mobility = board.mobility();
	if (mobility == 0) {
		moves.push_back(PASS);
	} else {
		for (pos_t pos = FIRST; pos < LAST; ++pos) {
			if (board.is_active(pos)) {
				moves.push_back(pos);
			}
		}
		//可以在这里随机丢掉一些节点，通过减少节点数提高深度，近似于蒙特卡洛思想
		//if (size>10) { shuffle, pop pop }
//		std::random_shuffle(moves.begin(), moves.end());
		assert(moves.size() == mobility);
	}
	index = 0;
	score = -1e100;
	best = PASS;
}

void State::update_score(double child_win) { //根据某个孩子状态的分数更新当前状态的分数
	double father_win= -child_win;//孩子赢多少自己就输多少 neg_max

	//alpha-beta剪枝
	if (father_win>beta) {
		score = father_win;
		best = this->get_move_pos();
		skip_all();
		return;
	}

	if (father_win > score) {
		score = father_win;
		best = this->get_move_pos();
		if (father_win>alpha) {
			alpha=father_win;
		}
	}

//	if (child_score>score) score=(score+child_score)/2;
//	if (child_score>score) score=(2*score+child_score)/3;
//	if (child_score>score) score=(score+2*child_score)/3;
//
//	score=(score+child_score)/2;
//
//	score+=child_score/moves.size();
//
//	score+=child_score;

}

ostream& State::dump(ostream& o) const {
	color self=board.turn();
	o << "State("
		<<"best="<<Move(self, best)
		<<", score="<<score
		<<", index="<<index
		<<", moves=[";
	for_n(i, moves.size()) {
		o<< i << ":" << Move(self, moves[i])<<" ";
	}
	return o<<"])";
}
