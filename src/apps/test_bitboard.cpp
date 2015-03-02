/*
 * File: test_bitboard.cpp
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月23日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "bitboard/board.h"

void test_game() {
	BitBoard board;

	board.start_game();
	board.init_bit_board();
	board.start_game();
}

void test_undo() {
	BitBoard board;
	log_status(board);
	log_status(move_db);
//	cout<<board.get_mobility()<<endl;

	cout<<board.search()<<endl;
	log_status(board);
	log_status(move_db);

	board.undo_move();
	log_status(board);

	cout<<board.search()<<endl;
	log_status(board);

	cout<<board.get_mobility()<<endl;
}

int main() {
//	init_valid_move_byte_table();

	for_n(j, 64) {
		timer now;
		BitBoard board;
		int i=j+1;
		board.rand_play(64-i);
//		cout<<board.negamax(i)<<endl;

//		cout<<board.alpha_beta(-64, 64, i)<<endl;
//		cout<<board.pvs(-64, 64, i)<<endl;
		cout<<board.mtd(0, i)<<endl;


//		log_status(move_db);
		log_status(i<<" levels, using "<<now.elapsed()<<"s");
	}

	return 0;
}

