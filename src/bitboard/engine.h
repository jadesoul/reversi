#ifndef ENGINE_H_1420019316_93
#define ENGINE_H_1420019316_93
/**
 * File: engine.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2014-12-31 17:48:36.925048
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"

bool check1(const char* s) {
	int n = strlen(s);
	if (n != 65)
		return false;
	for (int i = 0; i < 64; ++i) {
		if (!(s[i] == '0' || s[i] == '1' || s[i] == '2'))
			return false;
	}
	if (!(s[64] == '1' || s[64] == '2'))
		return false;
	return true;
}

const char* server(const char* s) {
	if (!check1(s))
		return "";

	BitBoard board;
	return board.deal(s).c_str();
}

#endif /* ENGINE_H_1420019316_93 */
