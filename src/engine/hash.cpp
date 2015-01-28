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
#include "pypp/random.h"

hash_t board_hash_table[MAP_SIZE+1][5];

void init_board_hash_table() {
	Random r;
	for_n(i, MAP_SIZE+1) {
		for_n(j, 5) {
			hash_t a=rand();
			hash_t b=rand();
			hash_t c=rand();
			hash_t h=(a<<32) ^ (b<<16) ^ c;
			board_hash_table[i][j]= h;
			log_status("board_hash_table["<<i<<"]["<<j<<"]=0x"<<std::hex<<h<<std::dec);
		}
	}
}
