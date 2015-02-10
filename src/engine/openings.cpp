/**
 * File: openings.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:59:36.080510
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "openings.h"

#include "player.h"
#include "human.h"
#include "game.h"

OpeningBook openings;

OpeningBook::OpeningBook() {
	init_board_hash_table();
	load("/Users/jadesoul/git/reversi/data/openingslarge.txt");
	load("/Users/jadesoul/git/reversi/data/openings.txt");
}

//从文件中加载开局库，根据字符串转换为棋局，自动做对称变换
void OpeningBook::load(const char* fp) {
	ifstream fin(fp);
	log_status("loading opening book from: "<<fp);
	string s;
	uint cnt = 0;
	while (getline(fin, s)) {
		HumanPlayer black, white;
		Game game(black, white);
		if (game.start_one_opening(s, book)) {
			++cnt;
		}
	}

	uint total_moves;
	for (book_t::iterator it = book.begin(); it != book.end(); ++it) {
		total_moves += it->second.size();
	}
	log_status("loaded opening book: "<<cnt<<" openings, now "<<book.size()<<" boards, "<<total_moves<<" moves in total");
	fin.close();
}

pos_t OpeningBook::lookup(const Board& board) const {
	book_t::const_iterator it = book.find(board.get_hash());
//	log_status("in OpeningBook::lookup(const Board& board)"<<board);
	if (it == book.end()) {
		log_status("found noting in openings book");
		return PASS;
	} else {

		const Choices& moves = it->second;
		uint n = moves.size();
		assert(n > 0);
		log_status(board);

		pos_t move = PASS;
		uint cnt = 0;
		double max_score = INT32_MIN;
		for (Choices::const_iterator it2 = moves.begin(); it2 != moves.end();
				++it2) {
			double score = it2->second;
			if (score > max_score) {
				max_score = score;
				move = it2->first;
			}
			++cnt;
			log_status("found opening "<<cnt<<" : "<<Move(board.turn(), it2->first));
		}

//			if (n==1) {//只有1种方案
//				move= moves.begin()->first;
//			} else {//多种方案，随机选择一个结果
//				uint index=random.randindex(cnt);
//				Choices::const_iterator it3=moves.begin();
//				while (index-- > 0) {
//					++it3;
//				}
//				move= it3->first;
//			}

		log_status("found "<<n<<" opening moves in opening book, choose move: "<<Move(board.turn(), move)<<", score="<<max_score);
		return move;
	}
	return PASS;
}
