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
class Move {
public:
	color turn;//谁下
	int pos;//落子位置
	double win;//估值

	Move(color turn=BLACK, int pos=PASS, double win=0):turn(turn), pos(pos), win(win) {}
	Move(const char two_bytes[2], double win=0) { init(two_bytes, win); }
	Move(const string& s, double win=0):Move(s.c_str(), win) {}

	inline bool is_illegal_move() { return turn==EMPTY and pos==ERROR_POS; }
	inline bool is_legal_move() { return !is_illegal_move(); }

	//将位置按照xy夹角对应的对角线镜像变换
	inline void mirror_xy() { pos=MIRROR_XY(pos); }

	//将位置按照左下-右上对角线镜像变换
	inline void mirror_ldru() { pos=MIRROR_IJ(pos); }

	//将位置按照左下-右上对角线镜像变换，同时按照xy夹角对应的对角线镜像变换
	inline void mirror_ldru_xy() { pos=MIRROR_IY(pos); }

	istream& from(istream& i=cin);
	friend inline istream& operator>>(istream& i, Move& m) { return m.from(i); }
	ostream& dump(ostream& o=cout) const;
	friend inline ostream& operator<<(ostream& o, const Move& m) { return m.dump(o); }

protected:
	void init(const char two_bytes[2], double win);
};

#endif /* MOVE_H_1421849524_37 */
