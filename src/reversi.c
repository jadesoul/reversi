/*
 * File: reversi.c
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月10日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "search.h"

void test_game() {
	init_board();
	start_game();
}

void test_undo() {
	search();
//	undo_move();
	search();
	int m;
	get_mobility(m);
	printf("%d\n", m);
}

void test_search() {
	for_n(j, 60) {
		init_board();
		TIMER_START(now);
		int i=j+1;
		if (rand_play(60-i)) {
			int win;

//			print_board();

			win=mtd(0, i);
	//		win=negamax(i, 0);
	//		win=alpha_beta(-64, 64, i, 0);
//			win=pvs(-64, 64, i, 0);

			printf("%d levels, using %f seconds, win=%d \n", i, TIMER_ELASPED(now), (turn==BLACK?win:-win));
		}
	}
}

void test_depening() {
	printf("%d\n", deepening(60));
}

int test() {
	int m;
	get_mobility(m);
	printf("%d\n", m);
	printf("%d\n", start_game());

	return 0;
}

int main() {
	init_board();

//	init_valid_move_byte_table();
//	test_undo();
	test_depening();

//	test_game();
//	test_search();

	return 0;
}
