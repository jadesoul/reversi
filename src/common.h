#ifndef COMMON_H_
#define COMMON_H_
/*
 * File: common.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年3月12日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

typedef uint8_t uchar;
typedef uint32_t uint;
typedef uint64_t ulong;

//棋盘边长
#define LEN				8
//总棋格数
#define SIZE			64	// (LEN * LEN)

//棋子的颜色状态
#define DRAW			0	//平局
#define BLACK			0	//黑子
#define WHITE			1	//白子
#define EMPTY			2	//空格
#define PASS			SIZE	//代表无子可下

#define COLOR(turn) 	((turn)==BLACK?"BLACK":"WHITE")

//给定自己棋子的颜色，获取对手的颜色
#define OPPO(c)			(BLACK+WHITE-(c))

//timer tools
#define TIMER_START(timer)		clock_t timer=clock();
#define TIMER_ELASPED(timer)	( (clock() - timer) * 1.0 / CLOCKS_PER_SEC )

//basic macros
#define ABS(x) 			((x) < 0 ? -(x) : (x))
#define MIN(a, b) 		((a)<(b) ? (a) : (b))
#define MAX(a, b) 		((a)<(b) ? (b) : (a))
#define SWAP(a, b)		{ int tmp=a; a=b; b=tmp; }
#define SWAP64(a, b)	{ ulong tmp=a; a=b; b=tmp; }

//给定二维坐标，获取一维坐标，都是从0开始
#define POS(i, j)		((i)*LEN + (j))

//给定一维坐标，获取二维坐标，都是从0开始
#define I(pos)			((pos) / LEN)
#define J(pos)			((pos) % LEN)

//给定二维坐标（从1开始），获取一维坐标（从0开始）
#define GRID(x, y)		POS((x-1), (y-1))

//定义棋盘上的各个位置符号
#define A1				GRID(1, 1) // 0
#define B1				GRID(1, 2)
#define C1				GRID(1, 3)
#define D1				GRID(1, 4)
#define E1				GRID(1, 5)
#define F1				GRID(1, 6)
#define G1				GRID(1, 7)
#define H1				GRID(1, 8)

#define A2				GRID(2, 1)
#define B2				GRID(2, 2)
#define C2				GRID(2, 3)
#define D2				GRID(2, 4)
#define E2				GRID(2, 5)
#define F2				GRID(2, 6)
#define G2				GRID(2, 7)
#define H2				GRID(2, 8)

#define A3				GRID(3, 1)
#define B3				GRID(3, 2)
#define C3				GRID(3, 3)
#define D3				GRID(3, 4)
#define E3				GRID(3, 5)
#define F3				GRID(3, 6)
#define G3				GRID(3, 7)
#define H3				GRID(3, 8)

#define A4				GRID(4, 1)
#define B4				GRID(4, 2)
#define C4				GRID(4, 3)
#define D4				GRID(4, 4)
#define E4				GRID(4, 5)
#define F4				GRID(4, 6)
#define G4				GRID(4, 7)
#define H4				GRID(4, 8)

#define A5				GRID(5, 1)
#define B5				GRID(5, 2)
#define C5				GRID(5, 3)
#define D5				GRID(5, 4)
#define E5				GRID(5, 5)
#define F5				GRID(5, 6)
#define G5				GRID(5, 7)
#define H5				GRID(5, 8)

#define A6				GRID(6, 1)
#define B6				GRID(6, 2)
#define C6				GRID(6, 3)
#define D6				GRID(6, 4)
#define E6				GRID(6, 5)
#define F6				GRID(6, 6)
#define G6				GRID(6, 7)
#define H6				GRID(6, 8)

#define A7				GRID(7, 1)
#define B7				GRID(7, 2)
#define C7				GRID(7, 3)
#define D7				GRID(7, 4)
#define E7				GRID(7, 5)
#define F7				GRID(7, 6)
#define G7				GRID(7, 7)
#define H7				GRID(7, 8)

#define A8				GRID(8, 1)
#define B8				GRID(8, 2)
#define C8				GRID(8, 3)
#define D8				GRID(8, 4)
#define E8				GRID(8, 5)
#define F8				GRID(8, 6)
#define G8				GRID(8, 7)
#define H8				GRID(8, 8) // 63

#define TEXT(pos)		'A'+J(pos), '1'+I(pos)

//棋盘左上角第一颗子的坐标
#define FIRST							A1
//棋盘右下角最后一颗子的坐标
#define LAST							H8

//一维坐标向东南西北四个方位进行快速变换
#define NORTH							(-LEN)
#define SOUTH							LEN
#define WEST							(-1)
#define EAST							1

#define TO_SOUTH(pos)					((pos) + SOUTH)
#define TO_NORTH(pos)					((pos) + NORTH)
#define TO_WEST(pos)					((pos) + WEST)
#define TO_EAST(pos)					((pos) + EAST)

#define SOUTH_WEST(pos)					((pos) + (SOUTH + WEST))
#define SOUTH_EAST(pos)					((pos) + (SOUTH + EAST))
#define NORTH_WEST(pos)					((pos) + (NORTH + WEST))
#define NORTH_EAST(pos)					((pos) + (NORTH + EAST))

#define SOUTH_SOUTH(pos)				((pos) + (SOUTH + SOUTH))
#define NORTH_NORTH(pos)				((pos) + (NORTH + NORTH))
#define WEST_WEST(pos)					((pos) + (WEST + WEST))
#define EAST_EAST(pos)					((pos) + (EAST + EAST))

//pos_mask代表一个下子点对应的64bits,只有对应的位置是1
#define MASK_EAST(pos_mask)				((pos_mask) << 1)
#define MASK_WEST(pos_mask)				((pos_mask) >> 1)
#define MASK_NORTH(pos_mask)			((pos_mask) >> LEN)
#define MASK_SOUTH(pos_mask)			((pos_mask) << LEN)

#define MASK_NORTH_EAST(pos_mask)		((pos_mask) >> (LEN-1))
#define MASK_NORTH_WEST(pos_mask)		((pos_mask) >> (LEN+1))
#define MASK_SOUTH_EAST(pos_mask)		((pos_mask) << (LEN+1))
#define MASK_SOUTH_WEST(pos_mask)		((pos_mask) << (LEN-1))

//64bit operations
#define ONE								0x0000000000000001LU//仅最低位是1
#define ALLONE							0xFFFFFFFFFFFFFFFFLU//所有的位全是1

#define BIT_EXIST(bits, pos)			( bits & (ONE << pos) ) // ( (bits >> pos) & 0x01 )
#define BIT_NOT_EXIST(bits, pos)		( ! BIT_EXIST(bits, pos) )

#define SET_BIT(bits, pos)				( bits |= (ONE << pos) )
#define CLEAR_BIT(bits, pos)			( bits &= (ALLONE ^ (ONE << pos) ) )
#define FLIP_BIT(bits, pos)				( bits ^= (ONE << pos) )
#define REVERSE_BITS(bits)				( bits ^= ALLONE )

#define IS_EMPTY(my, op, pos)			BIT_NOT_EXIST((my | op), pos)

// open v1: some app use this version
// #define OPEN_BLACK						((ONE << D4) | (ONE << E5))
// #define OPEN_WHITE						((ONE << D5) | (ONE << E4))

// open v2: most openings use this version
#define OPEN_BLACK						((ONE << D5) | (ONE << E4))
#define OPEN_WHITE						((ONE << D4) | (ONE << E5))

//utility macros
#define for_each_pos(p) 				for (int p=A1; p<=H8; ++p)
#define for_each_index(i) 				for (int i=played_cnt; i<64; ++i)
#define for_n(i, n) 					for (uint i=0; i<n; ++i)

//utility functions
uchar reverse_byte(uchar c) { // why not just use this: c ^= 0xFF
	c = ( c & 0x55 ) << 1 | ( c & 0xAA ) >> 1;
	c = ( c & 0x33 ) << 2 | ( c & 0xCC ) >> 2;
	c = ( c & 0x0F ) << 4 | ( c & 0xF0 ) >> 4;
	return c;
}

/*
  BIT_REVERSE_32
  Returns the bit-reverse of a 32-bit integer.
*/
unsigned int bit_reverse_32( unsigned int val ) {
	val = ((val >>  1) & 0x55555555) | ((val <<  1) & 0xAAAAAAAA);
	val = ((val >>  2) & 0x33333333) | ((val <<  2) & 0xCCCCCCCC);
	val = ((val >>  4) & 0x0F0F0F0F) | ((val <<  4) & 0xF0F0F0F0);
	val = ((val >>  8) & 0x00FF00FF) | ((val <<  8) & 0xFF00FF00);
	val = ((val >> 16) & 0x0000FFFF) | ((val << 16) & 0xFFFF0000);
	return val;
}

/*
  Counts the number of bits set in a 64-bit integer.
  This is done using some bitfiddling tricks.
*/
uint non_iterative_popcount_32(uint n) {
	n = n - ((n >> 1) & 0x55555555u);
	n = (n & 0x33333333u) + ((n >> 2) & 0x33333333u);
	n = (n + (n >> 4)) & 0x0F0F0F0Fu;
	return (n * 0x01010101u) >> 24;
}

#define non_iterative_popcount_64(n) (non_iterative_popcount_32(n>>32)+non_iterative_popcount_32(n & 0xFFFFFFFF))

/*
  Counts the number of bits set in a 64-bit integer.
  This is done using an iterative procedure which loops
  a number of times equal to the number of bits set,
  hence this function is fast when the number of bits
  set is low.
*/
uint iterative_popcount_32(uint x) {
	uint n = 0;
	while (x != 0) {
		++n;
		x &= (x - 1);
	}
	return n;
}

uint iterative_popcount_64(ulong x) {
	uint n=0;
	while (x!=0) {
		++n;
		x &= (x-1);
	}
	return n;
}

// TODO: if my < xx; then iterative ; other wise non-iterative


#endif /* COMMON_H_ */
