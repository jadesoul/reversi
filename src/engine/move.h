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

	Move(color turn=BLACK, int pos=PASS):turn(turn), pos(pos) {}
	Move(const char two_bytes[2]);
	Move(const string& s):Move(s.c_str()) {}

	inline bool is_illegal_move() { return turn==EMPTY and pos==ERROR_POS; }

	//将位置按照xy夹角对应的对角线镜像变换
	inline void mirror_xy() { pos=MIRROR_XY(pos); }

	//将位置按照左下-右上对角线镜像变换
	inline void mirror_ldru() { pos=MIRROR_IJ(pos); }

	//将位置按照左下-右上对角线镜像变换，同时按照xy夹角对应的对角线镜像变换
	inline void mirror_ldru_xy() { pos=MIRROR_IY(pos); }

	ostream& dump(ostream& o=cout) const;
	friend inline ostream& operator<<(ostream& o, const Move& m) { return m.dump(o); }
};

#endif /* MOVE_H_1421849524_37 */
