#ifndef BITBOARD_POS_H_
#define BITBOARD_POS_H_
/*
 * File: pos.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

//给定二维坐标，获取一维坐标，都是从0开始
#define POS(i, j)						((i)*LEN + (j))

//给定一维坐标，获取二维坐标，都是从0开始
#define I(pos)							((pos) / LEN)
#define J(pos)							((pos) % LEN)

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

#define INITIAL_BLACK_BITS		(D4 | E5)
#define INITIAL_WHITE_BITS		(D5 | E4)

#endif /* BITBOARD_POS_H_ */
