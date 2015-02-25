/*
 * File: test_bitboard.cpp
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月23日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"

typedef uint64_t ulong;

#define BIT_EXIST(bits, offset)		(((bits) >> (offset)) & 0x01)
#define SET_BIT(bits, offset)		((bits) |= (0x01 << (offset)))
//#define CLEAR_BIT(bits, offset)		((bits) &= (0x00 << (offset)))

#define OFFSET2(i, j)	OFFSET((i-1), (j-1))//从1开始的下标

#define A1				OFFSET2(1, 1)
#define B1				OFFSET2(1, 2)
#define C1				OFFSET2(1, 3)
#define D1				OFFSET2(1, 4)
#define E1				OFFSET2(1, 5)
#define F1				OFFSET2(1, 6)
#define G1				OFFSET2(1, 7)
#define H1				OFFSET2(1, 8)

#define A2				OFFSET2(2, 1)
#define B2				OFFSET2(2, 2)
#define C2				OFFSET2(2, 3)
#define D2				OFFSET2(2, 4)
#define E2				OFFSET2(2, 5)
#define F2				OFFSET2(2, 6)
#define G2				OFFSET2(2, 7)
#define H2				OFFSET2(2, 8)

#define A3				OFFSET2(3, 1)
#define B3				OFFSET2(3, 2)
#define C3				OFFSET2(3, 3)
#define D3				OFFSET2(3, 4)
#define E3				OFFSET2(3, 5)
#define F3				OFFSET2(3, 6)
#define G3				OFFSET2(3, 7)
#define H3				OFFSET2(3, 8)

#define A4				OFFSET2(4, 1)
#define B4				OFFSET2(4, 2)
#define C4				OFFSET2(4, 3)
#define D4				OFFSET2(4, 4)
#define E4				OFFSET2(4, 5)
#define F4				OFFSET2(4, 6)
#define G4				OFFSET2(4, 7)
#define H4				OFFSET2(4, 8)

#define A5				OFFSET2(5, 1)
#define B5				OFFSET2(5, 2)
#define C5				OFFSET2(5, 3)
#define D5				OFFSET2(5, 4)
#define E5				OFFSET2(5, 5)
#define F5				OFFSET2(5, 6)
#define G5				OFFSET2(5, 7)
#define H5				OFFSET2(5, 8)

#define A6				OFFSET2(6, 1)
#define B6				OFFSET2(6, 2)
#define C6				OFFSET2(6, 3)
#define D6				OFFSET2(6, 4)
#define E6				OFFSET2(6, 5)
#define F6				OFFSET2(6, 6)
#define G6				OFFSET2(6, 7)
#define H6				OFFSET2(6, 8)

#define A7				OFFSET2(7, 1)
#define B7				OFFSET2(7, 2)
#define C7				OFFSET2(7, 3)
#define D7				OFFSET2(7, 4)
#define E7				OFFSET2(7, 5)
#define F7				OFFSET2(7, 6)
#define G7				OFFSET2(7, 7)
#define H7				OFFSET2(7, 8)

#define A8				OFFSET2(8, 1)
#define B8				OFFSET2(8, 2)
#define C8				OFFSET2(8, 3)
#define D8				OFFSET2(8, 4)
#define E8				OFFSET2(8, 5)
#define F8				OFFSET2(8, 6)
#define G8				OFFSET2(8, 7)
#define H8				OFFSET2(8, 8)

class BitBoard {
public:
	BitBoard() {
		init_bit_board();
	}

	//pos : [0, 63]
	inline bool is_black_stone(uint pos) const { return BIT_EXIST(black_bits, pos); }
	inline bool is_white_stone(uint pos) const { return BIT_EXIST(white_bits, pos); }

	inline void set_black_stone(uint pos) const { SET_BIT(black_bits, pos); }
	inline void set_white_stone(uint pos) const { SET_BIT(white_bits, pos); }

	bool valid_move(uint pos) {
		//z
	}

	void init_bit_board() {
		black_bits=0;
		set_black_stone(D4);
		set_black_stone(E5);

		white_bits=0;
		set_white_stone(D5);
		set_white_stone(E4);

		black_cnt=2;
		white_cnt=2;

		total_move=0;
	}

private:
	//board map
	ulong black_bits;
	ulong white_bits;

	//counters
	uint black_cnt;
	uint white_cnt;

	//play history
	Move move_history[64];
	ulong flip_history[64];//flip bits mask, used for unplay
	uint total_move;//how many move have been played
};
int main() {
	return 0;
}

