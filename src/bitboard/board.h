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
#include "functor.h"
#include "pos.h"
#include "mask.h"
#include "log2.h"

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
	BitBoard() {
		init_bit_board();
	}

	//pos : [0, 63]
	inline bool is_black_stone(uint pos) const { return BIT_EXIST(black_bits, pos); }
	inline bool is_white_stone(uint pos) const { return BIT_EXIST(white_bits, pos); }

	inline void set_black_stone(uint pos) { SET_BIT(black_bits, pos); }
	inline void set_white_stone(uint pos) { SET_BIT(white_bits, pos); }

	//验证是否可以下子，pos:下子位置
	bool valid_move(ulong& my_bits, ulong& op_bits, const uint& pos) {
		mask_t pos_mask= 0x01 << pos;
		if (my_bits & pos_mask) return false;
		return valid_move_from_pos_functions[pos](my_bits, op_bits, pos_mask);
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
//	Move move_history[64];
	ulong flip_history[64];//flip bits mask, used for unplay
	uint total_move;//how many move have been played
};

#endif /* BITBOARD_BOARD_H_ */
