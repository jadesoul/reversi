#ifndef COMMON_H_1421849265_74
#define COMMON_H_1421849265_74
/**
 * File: common.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:07:45.741720
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "pypp/bootstrap.h"

typedef uint color;//颜色
typedef uint pos;//位置
typedef uint64_t ulong;
typedef ulong hash;

//代表在当前局面下，在每个位置下一步棋，当前下子方最终赢得棋子数的期望(大于0赢棋，小于0输棋)
typedef map<pos, int> choices;

//开局库数据格式，一个局面hash，对应若干个带分数的下法
typedef map<hash, choices> book;

//棋谱树
typedef map<hash, book> tree;

//棋盘边长
#define LEN								8
//总棋格数
#define SIZE							64// (LEN * LEN)

//棋子的颜色状态
#define DRAW							0//平局
#define EMPTY							0//空格
#define WHITE							1//白子 0b01
#define BLACK							2//黑子	0b10
#define PASS							SIZE//代表无子可下

//给定自己棋子的颜色，获取对手的颜色
#define OPPO(c)							(BLACK+WHITE-(c))

#include "pos.h"

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

#ifdef ENABLE_THIS
#define IS_COLOR(pos, color)	(BOARD(pos)==(color))
#define IS_WALL(pos)			IS_COLOR(pos, WALL)
#define IS_EMPTY(pos)			IS_COLOR(pos, EMPTY)
#define IS_BLACK(pos)			IS_COLOR(pos, BLACK)
#define IS_WHITE(pos)			IS_COLOR(pos, WHITE)
#define IS_EMPTY_OR_ACTIVE(pos) (BOARD(pos)<=ACTIVE)

//是否周围8个方向有任意一个邻居具有指定颜色
#define NEIGHBOR_HAS_COLOR(pos, color)	(BOARD(SOUTH(pos))==(color) \
										|| BOARD(WEST(pos))==(color) \
										|| BOARD(NORTH(pos))==(color) \
										|| BOARD(EAST(pos))==(color) \
										|| BOARD(SW(pos))==(color) \
										|| BOARD(NW(pos))==(color) \
										|| BOARD(NE(pos))==(color) \
										|| BOARD(SE(pos))==(color))

//是否周围右下3个方向有任意一个邻居具有指定颜色
#define NEIGHBOR_HAS_COLOR_FAST(pos, color)	(BOARD(SOUTH(pos))==(color) \
										|| BOARD(EAST(pos))==(color) \
										|| BOARD(SE(pos))==(color))

//根据与两条坐标轴夹角对角线对折后的新坐标
#define MIRROR_XY(pos)			POS(J(pos), I(pos))
//根据与两条坐标轴夹角对角线垂直的另一条对角线对折后的新坐标
#define MIRROR_IJ(pos)			POS(BOARD_LEN - 1 - J(pos), BOARD_LEN - 1 - I(pos))
//先后根据两条对角线对折后的新坐标
#define MIRROR_IY(pos)			POS(BOARD_LEN - 1 - I(pos), BOARD_LEN - 1 - J(pos))

#define END (uchar)(-1)
#define COLOR(turn) ((turn)==BLACK?"BLACK":"WHITE")

#define M		(BOARD_LEN-1)
#define LU		POS(0,0)
#define RU		POS(0,M)
#define LD		POS(M,0)
#define RD		POS(M,M)

#define IS_LU_CORNER(pos)			((pos)==LU)
#define IS_RU_CORNER(pos)			((pos)==RU)
#define IS_LD_CORNER(pos)			((pos)==LD)
#define IS_RD_CORNER(pos)			((pos)==RD)

//用于判断是否是四个角上的好手位置上
#define IS_GOOD_CORNER(pos)			(IS_LU_CORNER(pos) \
									|| IS_RU_CORNER(pos) \
									|| IS_LD_CORNER(pos) \
									|| IS_RD_CORNER(pos))

#define LU1		SE(LU)
#define LU2		SOUTH(LU)
#define LU3		EAST(LU)
#define RU1		SW(RU)
#define RU2		SOUTH(RU)
#define RU3		WEST(RU)
#define LD1		NE(LD)
#define LD2		NORTH(LD)
#define LD3		EAST(LD)
#define RD1		NW(RD)
#define RD2		NORTH(RD)
#define RD3		WEST(RD)

#define BESIDE_LU_CORNER(pos)		((pos)==LU1 or (pos)==LU2 or (pos)==LU3)
#define BESIDE_RU_CORNER(pos)		((pos)==RU1 or (pos)==RU2 or (pos)==RU3)
#define BESIDE_LD_CORNER(pos)		((pos)==LD1 or (pos)==LD2 or (pos)==LD3)
#define BESIDE_RD_CORNER(pos)		((pos)==RD1 or (pos)==RD2 or (pos)==RD3)

//用于判断是否是与四个角相邻的12个坏手位置上
#define BESIDE_GOOD_CORNER(pos)		(BESIDE_LU_CORNER(pos) \
									|| BESIDE_RU_CORNER(pos) \
									|| BESIDE_LD_CORNER(pos) \
									|| BESIDE_RD_CORNER(pos))

#define APPLY_8_DIRECTIONS(MACRO)	\
									MACRO(EAST) \
									MACRO(WEST) \
									MACRO(NORTH) \
									MACRO(SOUTH) \
									MACRO(NW) \
									MACRO(NE) \
									MACRO(SW) \
									MACRO(SE)

#define APPLY_8_PAIR_DIRECTIONS(MACRO)	\
									MACRO(EAST, WEST) \
									MACRO(WEST, EAST) \
									MACRO(NORTH, SOUTH) \
									MACRO(SOUTH, NORTH) \
									MACRO(NW, SE) \
									MACRO(NE, SW) \
									MACRO(SW, NE) \
									MACRO(SE, NW)

#define APPLY_4_TUPLE_DIRECTIONS(MACRO)	\
									MACRO(LU, EAST, SOUTH) \
									MACRO(RU, WEST, SOUTH) \
									MACRO(LD, EAST, NORTH) \
									MACRO(RD, WEST, NORTH)

#define APPLY_8_PAIR_DERECTION_AND_ENUM(MACRO)	\
									MACRO(EAST, LEFT) \
									MACRO(WEST, RIGHT) \
									MACRO(NORTH, DOWN) \
									MACRO(SOUTH, UP) \
									MACRO(NW, RIGHT_DOWN) \
									MACRO(NE, LEFT_DOWN) \
									MACRO(SW, RIGHT_UP) \
									MACRO(SE, LEFT_UP)



#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)<(b) ? (b) : (a))

//8个方向
#define RIGHT		1
#define RIGHT_DOWN	1<<1
#define DOWN		1<<2
#define LEFT_DOWN	1<<3
#define LEFT		1<<4
#define LEFT_UP		1<<5
#define UP			1<<6
#define RIGHT_UP	1<<7

//根据搜集得到的一个子周围的信息的8比特位判断是否是稳定子，即永远也不可能再被对手吃掉的子
#define IS_STABLE_STONE(BITS) \
			( ((BITS & LEFT) or (BITS & RIGHT)) \
			and ((BITS & UP) or (BITS & DOWN)) \
			and ((BITS & LEFT_UP) or (BITS & RIGHT_DOWN)) \
			and ((BITS & LEFT_DOWN) or (BITS & RIGHT_UP)) )

#endif

//位操作
#define BIT_EXIST(bits, offset)			(((bits) >> (offset)) & 0x01)
#define SET_BIT(bits, offset)			((bits) |= (0x01 << (offset)))
//#define CLEAR_BIT(bits, offset)		((bits) &= (0x00 << (offset)))

#endif /* COMMON_H_1421849265_74 */
