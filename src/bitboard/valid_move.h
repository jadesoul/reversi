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

#include "common.h"

//8个方向
#define RIGHT		0
#define RIGHT_DOWN	1
#define DOWN		2
#define LEFT_DOWN	3
#define LEFT		4
#define LEFT_UP		5
#define UP			6
#define RIGHT_UP	7

template<int DIRECTION>
class NextPosMaskChanger {
public:
	inline mask_t operator ()(const mask_t& pos_mask) {
		return (pos_mask << (DIRECTION * 0));
	}
	friend ostream& operator <<(ostream& o, NextPosMaskChanger& c) {
		return o<<"UndefinedNextPosMaskChanger";
	}
};

#define GEN_NEXT_POS_MASK_CHANGER(DIRECTION, MASK_CONV, NAME) 			\
template<>																\
class NextPosMaskChanger<DIRECTION> {									\
public:																	\
	inline mask_t operator ()(const mask_t& pos_mask) {					\
		return MASK_CONV(pos_mask);										\
	}																	\
	friend ostream& operator <<(ostream& o, NextPosMaskChanger& c) {	\
		return o<< #NAME ;												\
	}																	\
};																		\

GEN_NEXT_POS_MASK_CHANGER(RIGHT, 		MASK_EAST,			Right);
GEN_NEXT_POS_MASK_CHANGER(RIGHT_DOWN, 	MASK_SOUTH_EAST,	RightDown);
GEN_NEXT_POS_MASK_CHANGER(DOWN, 		MASK_SOUTH,			Down);
GEN_NEXT_POS_MASK_CHANGER(LEFT_DOWN, 	MASK_SOUTH_WEST,	LeftDown);
GEN_NEXT_POS_MASK_CHANGER(LEFT, 		MASK_WEST,			Left);
GEN_NEXT_POS_MASK_CHANGER(LEFT_UP, 		MASK_NORTH_WEST,	LeftUp);
GEN_NEXT_POS_MASK_CHANGER(UP, 			MASK_NORTH,			Up);
GEN_NEXT_POS_MASK_CHANGER(RIGHT_UP, 	MASK_NORTH_EAST,	RightUp);

typedef NextPosMaskChanger<0> Right;
typedef NextPosMaskChanger<1> RightDown;
typedef NextPosMaskChanger<2> Down;
typedef NextPosMaskChanger<3> LeftDown;
typedef NextPosMaskChanger<4> Left;
typedef NextPosMaskChanger<5> LeftUp;
typedef NextPosMaskChanger<6> Up;
typedef NextPosMaskChanger<7> RightUp;

class Dummy {
public:
	Dummy() {}

	friend ostream& operator <<(ostream& o, Dummy& d) {
		return o<<"Dummy";
	}
};

template<class PosMaskChanger, int LOOK_N, class NextChecker>
class ValidMoveFromPosChecker {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
//		log_status("in ValidMoveFromPosChecker: " << "Undefiend" << " LookN=" << LOOK_N );
		return NextChecker()(my, op, pmask);
	}
};

template<>
class ValidMoveFromPosChecker<Dummy, 0, Dummy> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
//		log_status("in ValidMoveFromPosChecker: " << "Dummy" << " LookN=" << 0 );

		return false;
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 0, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {

//		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<0);

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 1, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
//		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<1);

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 2, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<2);
		mask_t pm = next_pos_mask(pmask);

		if ((pm & op) and (next_pos_mask(pm) & my)) return true;
		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 3, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<3);
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if ((pm & op) and (next_pos_mask(pm) & my)) return true;
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 4, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<4);
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if ((pm & op) and (next_pos_mask(pm) & my)) return true;
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 5, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<5);
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if (pm & op) {
					pm = next_pos_mask(pm);
					if (pm & my) return true;
					if ((pm & op) and (next_pos_mask(pm) & my)) return true;
				}
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 6, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<6);
		mask_t pm = next_pos_mask(pmask);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if (pm & op) {
					pm = next_pos_mask(pm);
					if (pm & my) return true;
					if (pm & op) {
						pm = next_pos_mask(pm);
						if (pm & my) return true;
						if ((pm & op) and (next_pos_mask(pm) & my)) return true;
					}
				}
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

template<class PosMaskChanger, class NextChecker>
class ValidMoveFromPosChecker<PosMaskChanger, 7, NextChecker> {
public:
	inline bool operator ()(const ulong& my, const ulong& op, const mask_t& pmask) {
		PosMaskChanger next_pos_mask;
		mask_t pm = next_pos_mask(pmask);
//		log_status("in ValidMoveFromPosChecker: "<<next_pos_mask<<" LookN="<<7);

		if (pm & op) {
			pm = next_pos_mask(pm);
			if (pm & my) return true;
			if (pm & op) {
				pm = next_pos_mask(pm);
				if (pm & my) return true;
				if (pm & op) {
					pm = next_pos_mask(pm);
					if (pm & my) return true;
					if (pm & op) {
						pm = next_pos_mask(pm);
						if (pm & my) return true;
						if (pm & op) {
							pm = next_pos_mask(pm);
							if (pm & my) return true;
							if ((pm & op) and (next_pos_mask(pm) & my)) return true;
						}
					}
				}
			}
		}

		return NextChecker()(my, op, pmask);
	}
};

//check valid move from grid(x, y), pos(x-1, y-1)
//i, j starts from 0
//x, y starts from 1
template<uint x, uint y>
bool valid_move_checker(const ulong& my, const ulong& op, const mask_t& pmask) {

//	log_status("in valid_move_checker, x="<<x<<", y="<<y);

	typedef ValidMoveFromPosChecker<Dummy, 0, Dummy> 					End;
	typedef ValidMoveFromPosChecker<RightUp, MIN(8-y, x-1), End> 		A;
	typedef ValidMoveFromPosChecker<Up, x-1, A> 						B;
	typedef ValidMoveFromPosChecker<LeftUp, MIN(y-1, x-1), B> 			C;
	typedef ValidMoveFromPosChecker<Left, y-1, C> 						D;
	typedef ValidMoveFromPosChecker<LeftDown, MIN(y-1, 8-x), D> 		E;
	typedef ValidMoveFromPosChecker<Down, 8-x, E> 						F;
	typedef ValidMoveFromPosChecker<RightDown, MIN(8-y, 8-x), F> 		G;
	typedef ValidMoveFromPosChecker<Right, 8-y, G> 						H;

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

class ValidMoveChecker {
public:
	inline bool operator()(const ulong& my_bits, const ulong& op_bits, const uint& pos) {
		assert(IS_EMPTY(my_bits, op_bits, pos));
		mask_t pos_mask= ONE << pos;
		return valid_move_from_pos_functions[pos](my_bits, op_bits, pos_mask);
	}
};

ValidMoveChecker check_valid_move;




#endif /* BITBOARD_VALID_MOVE_H_ */
