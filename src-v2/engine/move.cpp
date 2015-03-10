/**
 * File: move.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:12:08.826285
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "move.h"

ostream& Move::dump(ostream& o) const {
	if (pos == PASS)
		return o << (turn == BLACK ? "PASS" : "pass");
	char base = (turn == BLACK ? 'A' : 'a');
	char h = base + J(pos);
	char v = '1' + I(pos);
	return o << h << v <<" "<<win;
}

istream& Move::from(istream& i) {
	string s;
	double w;
	i>>s>>w;
	init(s.c_str(), w);
	return i;
}

void Move::init(const char two_bytes[2], double win) {
	turn=EMPTY;
	pos=ERROR_POS;
	this->win=win;

	char h = two_bytes[0]; //水平方向
	char v = two_bytes[1]; //垂直方向
	if (v >= '1' and v <= '8') {
		int x = v - '1', y;
		if (h >= 'a' and h <= 'h') {
			y = h - 'a';
			turn = WHITE;
		} else if (h >= 'A' and h <= 'H') {
			y = h - 'A';
			turn = BLACK;
		}
		pos = POS(x, y);
	}
}
