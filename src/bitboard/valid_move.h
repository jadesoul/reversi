#ifndef BITBOARD_VALID_MOVE_H_
#define BITBOARD_VALID_MOVE_H_
/*
 * File: valid_move.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "mask.h"
#include "functor.h"

//check valid move from grid(x, y), pos(x-1, y-1)
//i, j starts from 0
//x, y starts from 1
template<uint x, uint y>
bool valid_move_checker(const ulong& my, const ulong& op, const mask_t& pmask) {
	typedef ValidMoveFromPosChecker<Dummy, 0, Dummy> 	End;
	typedef ValidMoveFromPosChecker<Right, 8-y, End> 	A;
	typedef ValidMoveFromPosChecker<Left, y-1, A> 		B;
	typedef ValidMoveFromPosChecker<Up, x-1, B> 		C;
	typedef ValidMoveFromPosChecker<Down, 8-x, C> 		D;

#define MIN(x, y) ((x)<(y)?(x):(y))

	typedef ValidMoveFromPosChecker<LeftUp, MIN(y-1, x-1), D> 		E;
	typedef ValidMoveFromPosChecker<LeftDown, MIN(y-1, 8-x), E> 	F;
	typedef ValidMoveFromPosChecker<RightUp, MIN(8-y, x-1), F> 		G;
	typedef ValidMoveFromPosChecker<RightDown, MIN(8-y, 8-x), G> 	H;

	return H()(my, op, pmask);
}

typedef bool (* ValidMoveFromPosFunction) \
		(const ulong& my_bits, const ulong& op_bits, const mask_t& pos_mask);

#define VM(x, y) valid_move_checker<x, y>

ValidMoveFromPosFunction valid_move_from_pos_functions[64]={
	VM(1,1), VM(1,2), VM(1,3), VM(1,4), VM(1,5), VM(1,6), VM(1,7), VM(1,8),
	VM(2,1), VM(2,2), VM(2,3), VM(2,4), VM(2,5), VM(2,6), VM(2,7), VM(2,8),
	VM(3,1), VM(3,2), VM(3,3), VM(3,4), VM(3,5), VM(3,6), VM(3,7), VM(3,8),
	VM(4,1), VM(4,2), VM(4,3), VM(4,4), VM(4,5), VM(4,6), VM(4,7), VM(4,8),
	VM(5,1), VM(5,2), VM(5,3), VM(5,4), VM(5,5), VM(5,6), VM(5,7), VM(5,8),
	VM(6,1), VM(6,2), VM(6,3), VM(6,4), VM(6,5), VM(6,6), VM(6,7), VM(6,8),
	VM(7,1), VM(7,2), VM(7,3), VM(7,4), VM(7,5), VM(7,6), VM(7,7), VM(7,8),
	VM(8,1), VM(8,2), VM(8,3), VM(8,4), VM(8,5), VM(8,6), VM(8,7), VM(8,8),
};

#endif /* BITBOARD_VALID_MOVE_H_ */
