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
		Board b;
		b.dump();
		cout<<"b.play(2, 3)="<<b.play(2, 3)<<endl;
		b.dump();
		cout<<"b.play(2, 4)="<<b.play(2, 4)<<endl;
		b.dump();
		cout<<"b.play(4, 5)="<<b.play(4, 5)<<endl;
		b.dump();

		cout<<"b.play(0, 0)="<<b.play(0, 0)<<endl;
		b.dump();
	}

	void setTest() {
		Board b;
		b.dump();
		b.set(0, 7, BLACK);
		cout<<"b.set(0, 7, BLACK);"<<endl;
		b.dump();
		b.set(6, 2, WHITE);
		cout<<"b.set(6, 2, WHITE);"<<endl;
		b.dump();
	}
};

int main () {
	openings = new OpeningBook();
	init_board_hash_table();
	BoardTest test;
	delete openings;
}

