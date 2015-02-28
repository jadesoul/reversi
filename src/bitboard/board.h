#ifndef BITBOARD_BOARD_H_
#define BITBOARD_BOARD_H_
/*
 * File: board.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"
#include "valid_move.h"
#include "make_move.h"

/**
black=		01111101110...
white=		00000010001...
mask=		10000000000...

mask-1=		01111111111

---
black=		01111110
white=		00000001
mask=		10000000
mask-1=		01111111

---
black=		01111100
white=		00000011
mask=		10000000
mask-1=		01111111

---
black=		01101100
white=		00000011
mask=		00010000
mask-1=		00001111

 */

uchar reverse_byte(uchar c)
{
	c = ( c & 0x55 ) << 1 | ( c & 0xAA ) >> 1;
	c = ( c & 0x33 ) << 2 | ( c & 0xCC ) >> 2;
	c = ( c & 0x0F ) << 4 | ( c & 0xF0 ) >> 4;
	return c;
}

//判断棋盘上某一行对应的位字节能否构成吃子(下子点右侧)
bool valid_move_byte_right(uchar my_byte, uchar op_byte, uchar pos/*[0, 8)*/) {
#ifdef ALGO1
	uchar pos_mask_byte = 0x01 << pos;

	//没子了
	if (my_byte==0 or op_byte==0) return false;

	//是否合法: 下子不能有交集
	if ((my_byte & op_byte) !=0) return false;

	//要求下子处是空格
	if ((my_byte & pos_mask_byte) != 0) return false;
	if ((op_byte & pos_mask_byte) != 0) return false;

	//右侧能否吃子
	uchar right_enable_mask = pos_mask_byte - 1;
	uchar right_my_byte = my_byte & right_enable_mask;
	int right = fast_log2_u8_table[right_my_byte];
	uchar right_eat_bits = (right_enable_mask >> right) << right;
	bool right_can_eat = (op_byte & right_eat_bits) == right_eat_bits;
	if (right_can_eat) return right_can_eat;

	//反转字节
	my_byte=reverse_byte(my_byte);
	op_byte=reverse_byte(op_byte);
	pos_mask_byte=0x01 << (7-pos);

	//左侧能否吃子
	uchar left_enable_mask = pos_mask_byte - 1;
	uchar left_my_byte = my_byte & left_enable_mask;
	int left = fast_log2_u8_table[left_my_byte];
	uchar left_eat_bits = (left_enable_mask >> left) << left;
	bool left_can_eat = (op_byte & left_eat_bits) == left_eat_bits;
	return left_can_eat;
#else
	return false;
#endif
}

//快速判断一行上是否能吃子
//预先存放256*256*8个bits，指示对应的可能性是否能吃子：共256*256个字节 my_byte * op_byte
static uchar valid_move_byte_table[256*256];

typedef bitset<8> byte_bitset;

void init_valid_move_byte_table() {
	for_n(my_byte, 256) {
		for_n(op_byte, 256) {
			uint index=(my_byte << 8) + op_byte;
			valid_move_byte_table[index]=0;
			for_n(pos, 8) {
				uchar pos_mask_byte=0x01 << pos;

				if (valid_move_byte_right(my_byte, op_byte, pos)) {
					valid_move_byte_table[index] |= pos_mask_byte;
					log_status(byte_bitset(my_byte)<<" , "<<byte_bitset(op_byte)<<" : "<<byte_bitset(pos_mask_byte)<<" = 1");
				} else {
					log_status(byte_bitset(my_byte)<<" , "<<byte_bitset(op_byte)<<" : "<<byte_bitset(pos_mask_byte)<<" = 0");
				}
			}
			log_status(byte_bitset(my_byte)<<" , "<<byte_bitset(op_byte)<<"\t=\t"<<byte_bitset(valid_move_byte_table[index]));
		}
	}
}

#define VALID_MOVE_BYTE(my_byte, op_byte, pos_mask_byte) \
	(valid_move_byte_table[(my_byte << 8) + op_byte] & pos_mask_byte)


class BitBoard {
public:
	BitBoard() { init_bit_board(); }

	//test color at pos : [0, 63]
	inline bool is_black(uint pos) const { return BIT_EXIST(black_bits, pos); }
	inline bool is_white(uint pos) const { return BIT_EXIST(white_bits, pos); }
	inline bool is_empty(ulong b, ulong w, uint pos) const { return BIT_NOT_EXIST((b | w), pos); }
	inline bool is_empty(uint pos) const { return BIT_NOT_EXIST((black_bits | white_bits), pos); }
	inline bool is_stone(uint pos) const { return is_black(pos) or is_white(pos); }

	//set color at pos
	inline void set_black(uint pos) { SET_BIT(black_bits, pos); CLEAR_BIT(white_bits, pos); }
	inline void set_white(uint pos) { CLEAR_BIT(black_bits, pos); SET_BIT(white_bits, pos); }
	inline void set_empty(uint pos) { CLEAR_BIT(black_bits, pos); CLEAR_BIT(white_bits, pos); }

	//flip color at pos
	inline void flip(uint pos) { assert(is_stone(pos)); FLIP_BIT(black_bits, pos); FLIP_BIT(white_bits, pos); }

	//test if we can make move at pos
	inline bool valid_move(const ulong& my, const ulong& op, const uint& pos) { return check_valid_move(my, op, pos); }

	//test if game is over
	inline bool game_over() { return empty_cnt==0 or pass_cnt>=2; }

	//you can call valid_move before this or not
	//using eat_table to cache the previous eating mask
	//if we can make move, return true
	inline bool make_move(ulong& my, ulong& op, const uint& pos) {
		assert(is_empty(my, op, pos));

		eat_val val;

		//if exist move in db
		if (! move_db.find(my, op, pos, val)) {
			//begin find move
			if (! move_maker.make_move(my, op, pos, val)) return false;

			//save new move into db
			move_db.add(my, op, pos, val);
		}

		//backup last my/op or last eat_mask
		//TODO

		//play move
		op ^= val.eat_mask;
		my ^= val.eat_mask;
		SET_BIT(my, pos);

		return true;
	}

	bool undo_move() {
		//recover from last
		//TODO
		return false;
	}

	void init_bit_board() {
		black_bits=INITIAL_BLACK_BITS;
		white_bits=INITIAL_WHITE_BITS;

		black_cnt=2;
		white_cnt=2;
		empty_cnt=60;
		pass_cnt=0;

		total_move=0;
	}

public:
	//board map
	ulong black_bits;
	ulong white_bits;

	//counters
	uint black_cnt;
	uint white_cnt;
	uint empty_cnt;
	uint pass_cnt;

	//play history
//	Move move_history[64];
	ulong flip_history[64];//flip bits mask, used for unplay
	uint total_move;//how many move have been played
};

#endif /* BITBOARD_BOARD_H_ */
