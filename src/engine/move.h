#ifndef MOVE_H_1421849524_37
#define MOVE_H_1421849524_37
/**
 * File: move.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:12:04.368971
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"

//一步棋，包括落子位置，轮谁下等
struct Move {
	color turn;
	int pos;

	Move(color turn=BLACK, int pos=PASS):turn(turn), pos(PASS) {}
	Move(const char* two_bytes[2]);

	ostream& dump(ostream& o=cout) const;
	friend inline ostream& operator<<(ostream& o, const Move& m) { return m.dump(o); }
};

#endif /* MOVE_H_1421849524_37 */
