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

#define TIME_START(timer)		clock_t timer=clock();
#define TIME_ELASPED(timer)		(double(clock() - timer) / CLOCKS_PER_SEC)

typedef uint32_t uint;
typedef uint64_t ulong;//64bits
typedef uint16_t ushort;
typedef uint8_t uchar;

//typedef uint color;//颜色
//typedef uint pos_t;//位置

//typedef ulong hash_t;//哈希
//typedef ulong mask_t;//掩码

//棋盘边长
#define LEN				8
//总棋格数
#define SIZE			64// (LEN * LEN)

//棋子的颜色状态
#define DRAW			0//平局
#define BLACK			0//黑子
#define WHITE			1//白子
#define EMPTY			2//空格
#define PASS			SIZE//代表无子可下

#define COLOR(turn) 	((turn)==BLACK?"BLACK":"WHITE")
#define BITS_TEXT(mask)	Mask(mask)

//给定自己棋子的颜色，获取对手的颜色
#define OPPO(c)			(BLACK+WHITE-(c))

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

//给定二维坐标（从1开始），获取一维坐标，（从0开始）
#define GRID(x, y)		POS((x-1), (y-1))

//定义棋盘上的各个位置符号
#define A1				GRID(1, 1)
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
#define H8				GRID(8, 8)

#define TEXT(pos)		char('A'+J(pos)) << char('1'+I(pos))


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

#define SOUTH_WEST(pos)					((pos) + SOUTH + WEST)
#define SOUTH_EAST(pos)					((pos) + SOUTH + EAST)
#define NORTH_WEST(pos)					((pos) + NORTH + WEST)
#define NORTH_EAST(pos)					((pos) + NORTH + EAST)

#define SOUTH_SOUTH(pos)				((pos) + 2 * SOUTH)
#define NORTH_NORTH(pos)				((pos) + 2 * NORTH)
#define WEST_WEST(pos)					((pos) + 2 * WEST)
#define EAST_EAST(pos)					((pos) + 2 * EAST)


//直接邻居
#define DIRECT_NEIGHBORS(pos1, pos2)		\
			((pos1) == SOUTH(pos2)			\
			|| (pos1) == WEST(pos2)			\
			|| (pos1) == NORTH(pos2)		\
			|| (pos1) == EAST(pos2))

//对角邻居
#define DIAGONAL_NEIGHBORS(pos1, pos2)		\
			((pos1) == SW(pos2)				\
			|| (pos1) == NW(pos2)			\
			|| (pos1) == NE(pos2)			\
			|| (pos1) == SE(pos2))


//pos_mask代表一个下子点对应的64bits,只有对应的位置是1，如果mask变为0了，说明位置出界了
#define MASK_EAST(pos_mask)				((pos_mask) << 1)//注意。。。
#define MASK_WEST(pos_mask)				((pos_mask) >> 1)//注意，下一排的第一个会循环到上一排的最后一个
#define MASK_NORTH(pos_mask)			((pos_mask) >> 8)
#define MASK_SOUTH(pos_mask)			((pos_mask) << 8)

#define MASK_NORTH_EAST(pos_mask)		((pos_mask) >> 7)
#define MASK_NORTH_WEST(pos_mask)		((pos_mask) >> 9)
#define MASK_SOUTH_EAST(pos_mask)		((pos_mask) << 9)
#define MASK_SOUTH_WEST(pos_mask)		((pos_mask) << 7)

//64bit operations
#define ONE								0x0000000000000001//仅最低位是1
#define ALLONE							0xFFFFFFFFFFFFFFFF//所有的位全是1

//#define BIT_EXIST(bits, offset)		( (bits >> offset) & 0x01 )
#define BIT_EXIST(bits, offset)			( bits & (ONE << offset) )
#define BIT_NOT_EXIST(bits, offset)		( ! BIT_EXIST(bits, offset) )

#define SET_BIT(bits, offset)			( bits |= (ONE << offset) )
#define CLEAR_BIT(bits, offset)			( bits &= (ALLONE ^ (ONE << offset) ) )
#define FLIP_BIT(bits, offset)			( bits ^= (ONE << offset) )
#define REVERSE_BITS(bits)				( bits ^= ALLONE )

#define IS_EMPTY(my, op, pos)			BIT_NOT_EXIST((my | op), pos)

#define OPEN_BLACK						((ONE << D4) | (ONE << E5))
#define OPEN_WHITE						((ONE << D5) | (ONE << E4))

//global variables
int turn, oppo;
ulong my, op;
int my_cnt, op_cnt;
int pass_cnt, empty_cnt, played_cnt;

//typedef struct move_t {
//	int 	turn;//who play//no need for use pass
//	int 	pos;//play at where// no need for undo_move(pos)
//	int		win;//how is the play//no need
//	ulong	eat;//flip which stones, used for unplay
//	int		total;//total eat cnt
//} move;

typedef ulong move;

move history[64];//played history, used for unplay
int	sequence[64];//maintain played moves index in the head of this sequences

#define MARK_SEQUENCE(pos)			SWAP(sequence[pos], sequence[played_cnt])
#define UNDO_SEQUENCE(pos)			SWAP(sequence[pos], sequence[played_cnt])//TODO

//get counters
#define BLACK_CNT			(turn==BLACK ? my_cnt : op_cnt)
#define WHITE_CNT			(turn==WHITE ? my_cnt : op_cnt)
#define STONE_CNT(color)	(color==BLACK ? BLACK_CNT : WHITE_CNT)

//get bits
#define BLACK_BITS			(turn==BLACK ? my : op)
#define WHITE_BITS			(turn==WHITE ? my : op)

//swap turn
#define SWAP_TURN(...)		SWAP(turn, oppo)

//test color at pos
#define is_my(pos)			(BIT_EXIST(my, pos))
#define is_op(pos)			(BIT_EXIST(op, pos))
#define is_black(pos)		(BIT_EXIST(BLACK_BITS, pos))
#define is_white(pos)		(BIT_EXIST(WHITE_BITS, pos))
#define is_stone(pos)		(is_black(pos) || is_white(pos))
#define is_empty(pos)		(IS_EMPTY(my, op, pos))

//get color at pos
#define get_color(pos)		(is_black(pos) ? BLACK : (is_white(pos) ? WHITE : EMPTY))

//set color at pos
#define set_my(pos)			{ SET_BIT(my, pos); CLEAR_BIT(op, pos); }
#define set_op(pos)			{ SET_BIT(op, pos); CLEAR_BIT(my, pos); }
#define set_black(pos)		{ SET_BIT(BLACK_BITS, pos); CLEAR_BIT(WHITE_BITS, pos); }
#define set_white(pos)		{ CLEAR_BIT(BLACK_BITS, pos); SET_BIT(WHITE_BITS, pos); }
#define set_empty(pos)		{ CLEAR_BIT(my, pos); CLEAR_BIT(op, pos); }

//flip color at pos
//#define flip(pos)			{ if (is_stone(pos)) { FLIP_BIT(my, pos); FLIP_BIT(op, pos); } }

//test if game is over
#define game_over(...)		(empty_cnt==0 || pass_cnt>=2)

//get mobility
#define get_my_mobility(mobility) 	{for (int pos=A1; pos<=H8; ++pos) if (my_valid_move(pos)) ++mobility;}
#define get_op_mobility(mobility) 	{for (int pos=A1; pos<=H8; ++pos) if (op_valid_move(pos)) ++mobility;}
#define get_mobility(mobility)		get_my_mobility(mobility)

//utility macros
#define for_each_pos(p) 			for (int p=A1; p<=H8; ++p)
#define for_each_index(i) 			for (int i=played_cnt; i<64; ++i)
#define for_n(i, n) 				for (uint i=0; i<n; ++i)

//utility functions
uchar reverse_byte(uchar c) {
	c = ( c & 0x55 ) << 1 | ( c & 0xAA ) >> 1;
	c = ( c & 0x33 ) << 2 | ( c & 0xCC ) >> 2;
	c = ( c & 0x0F ) << 4 | ( c & 0xF0 ) >> 4;
	return c;
}

//return the exact score
#define get_exact() 		(my_cnt - op_cnt)
//int get_exact() {
//		log_status("get_exact: "<< int(total[BLACK]-total[WHITE]));
//	return ;
//}

#define evaluation()		(my_cnt - op_cnt)
//int evaluation() {
//	win=;
//		win=get_mobility() - get_oppo_mobility();
//		log_status("got evaluation: "<< win);
//	return win;
//}


#endif /* COMMON_H_ */
