/**
 * File: hash.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:58:35.217303
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "hash.h"

hash_t board_hash_table[MAP_SIZE+1][5];

void init_board_hash_table() {
	srand(time(NULL));
	for_n(i, MAP_SIZE+1) {
		for_n(j, 5) {
			uint a=rand();
			uint b=rand();
			uint c=rand();
			board_hash_table[i][j]= a<<32+b<<16+c;
		}
	}
}
