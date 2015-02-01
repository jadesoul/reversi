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

typedef uchar color;//颜色
typedef uchar pos_t;//位置
typedef pos_t move_t;
//代表在当前局面下，在每个位置下一步棋，当前下子方最终赢得棋子数的期望(大于0赢棋，小于0输棋)
typedef map<pos_t, double> Choices;

//棋子的颜色状态
#define DRAW	0//平局
#define EMPTY	0//空格
#define ACTIVE	1//激活状态，可下子
#define WHITE	2//白子
#define BLACK	3//黑子
#define PASS	4//代表无子可下
#define WALL	4//墙壁，用于判断棋子出界

//给定自己棋子的颜色，获取对手的颜色
#define OPPO(c) (BLACK+WHITE-(c))

//棋盘边长
#define BOARD_LEN		8
//总棋格数
#define BOARD_SIZE		(BOARD_LEN * BOARD_LEN)

/*棋盘上方、左方、下方各加上一面墙后形成地图
 * 	墙上的子姑且叫墙子
 *	墙用来判断棋子是否出界
 *	末尾追加一个墙子，用于判断棋盘上最后一颗子的右下方边界
 */
//地图的宽度
#define MAP_WIDTH 		(BOARD_LEN + 1)
//地图的高度
#define MAP_HEIGHT 		(BOARD_LEN + 2)
//地图的总棋格数
#define MAP_SIZE 		(MAP_WIDTH * MAP_HEIGHT + 1)
//去掉首尾行墙壁后的主体部分棋格数
#define BOARD_BODY		(MAP_WIDTH * BOARD_LEN)

//去掉第一行的墙，和第二行的第一个墙子，棋盘上第一个棋子在地图上的偏移量
#define FIRST			(MAP_WIDTH + 1)
//去掉第一行墙，和所有棋子，棋盘最后一颗棋子的偏移量
#define LAST			(MAP_WIDTH + BOARD_BODY)

//是否是棋子颜色（而非辅助性颜色）
#define IS_STONE(color)	((color)==WHITE || (color)==BLACK)

//Note that POS(-1, -1) == 0
#define POS(i, j)		(FIRST + (i) * MAP_WIDTH + (j))
#define OFFSET(i, j)	((i)*BOARD_LEN + (j))
//DELTA() is defined so that POS(i+di, j+dj) = POS(i, j) + DELTA(di, dj).
#define DELTA(di, dj)	((di) * MAP_WIDTH + (dj))
#define I(pos)			((pos) / MAP_WIDTH - 1)
#define J(pos)			((pos) % MAP_WIDTH - 1)
#define ERROR_POS		MAP_SIZE
#define N				(-MAP_WIDTH)
#define S				MAP_WIDTH
#define W				(-1)
#define E				1
#define SOUTH(pos)		((pos) + S)
#define NORTH(pos)		((pos) + N)
#define WEST(pos)		((pos) + W)
#define EAST(pos)		((pos) + E)
#define SW(pos)			((pos) + S + W)
#define SE(pos)			((pos) + S + E)
#define NW(pos)			((pos) + N + W)
#define NE(pos)			((pos) + N + E)
#define SS(pos)			((pos) + 2 * S)
#define NN(pos)			((pos) + 2 * N)
#define WW(pos)			((pos) + 2 * W)
#define EE(pos)			((pos) + 2 * E)
#define	DELTA_8			N, N+E, E, S+E, S, S+W, W, N+W

#define ON_BOARD2(i, j)	((unsigned)(i)<BOARD_LEN and (unsigned)(j)<BOARD_LEN)
#define ON_BOARD(pos)	ON_BOARD2(I(pos), J(pos))

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

#define BOARD(pos)				data[pos]
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


#define ABS(x) ((x) < 0 ? -(x) : (x))
#define MIN(a, b) ((a)<(b) ? (a) : (b))
#define MAX(a, b) ((a)<(b) ? (b) : (a))

//8个方向
#define RIGHT		0
#define RIGHT_DOWN	1
#define DOWN		2
#define LEFT_DOWN	3
#define LEFT		4
#define LEFT_UP		5
#define UP			6
#define RIGHT_UP	7

#endif /* COMMON_H_1421849265_74 */
