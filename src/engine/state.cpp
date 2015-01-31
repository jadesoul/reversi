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

State::State(const Board& board) {
	moves.reserve(24);
	this->board = board;
	size_t mobility = board.mobility();
	if (mobility == 0) {
		moves.push_back(PASS);
	} else {
		for (pos_t pos = FIRST; pos < LAST; ++pos) {
			if (board.is_active(pos)) {
				moves.push_back(pos);
			}
		}
		std::random_shuffle(moves.begin(), moves.end());
		assert(moves.size() == mobility);
	}
	index = 0;
	score = -8888;
	best = PASS;
}

void State::update_score(double child_score) { //根据某个孩子状态的分数更新当前状态的分数
	if (child_score > score) {
		score = child_score;
		best = this->get_move_pos();
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
