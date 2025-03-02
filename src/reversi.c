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
	start_game(1);
}

void bench_mark(int verbose) {
	int n = 1000000;
	for (int i=0; i<n; i++) {
		if (verbose) printf("benchmark: %d / %d \n", i+1, n);
		init_board();
		start_game(0);
	}
}

void test_undo() {
	print_board();

	search();
	print_board();
	search();
	print_board();

	printf("now undo \n");
	undo_move(sequence[played_cnt-1]);
	print_board();

	printf("now undo \n");
	undo_move(sequence[played_cnt-1]);
	print_board();

	search();
	print_board();
}

void test_search() {
	for_n(j, 60) {
		init_board();
		TIMER_START(now);
		int i=j+1;
		if (rand_play(60-i)) {
			int win;

			print_board();

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
	printf("%d\n", start_game(1));

	return 0;
}

int main() {
	init_board();

	// init_valid_move_byte_table();
	// test_undo();
	// test_depening();

	// test_game();
	// test_search();

	bench_mark(0);
	return 0;
}
