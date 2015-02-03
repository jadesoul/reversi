/**
 * File: board.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 21:57:01.518995
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/engine.h"

class BoardTest : public Board {
public:
	BoardTest() {
		cout<<"sizeof(Board)="<<sizeof(Board)<<endl;
		cout<<"sizeof(BoardTest)="<<sizeof(BoardTest)<<endl;
		setTest();
		playTest();
	}

	void playTest() {
		init_board_map();
		dump();
		cout<<"b.play(2, 3)="<<play(2, 3)<<endl;
		dump();
		cout<<"b.play(2, 4)="<<play(2, 4)<<endl;
		dump();
		cout<<"b.play(4, 5)="<<play(4, 5)<<endl;
		dump();

		cout<<"b.play(0, 0)="<<play(0, 0)<<endl;
		dump();
	}

	void setTest() {
		init_board_map();
		dump();
		set(0, 7, BLACK);
		cout<<"b.set(0, 7, BLACK);"<<endl;
		dump();
		set(6, 2, WHITE);
		cout<<"b.set(6, 2, WHITE);"<<endl;
		dump();
	}
};

int main () {
//	openings = new OpeningBook();
	init_board_hash_table();
	BoardTest test;
//	delete openings;
}

