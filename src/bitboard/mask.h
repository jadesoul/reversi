#ifndef BITBOARD_MASK_H_
#define BITBOARD_MASK_H_
/*
 * File: mask.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

//pos_mask代表一个下子点对应的64bits,只有对应的位置是1，如果mask变为0了，说明位置出界了
#define MASK_EAST(pos_mask)				((pos_mask) >> 1)//注意。。。
#define MASK_WEST(pos_mask)				((pos_mask) << 1)//注意，下一排的第一个会循环到上一排的最后一个
#define MASK_NORTH(pos_mask)			((pos_mask) >> 8)
#define MASK_SOUTH(pos_mask)			((pos_mask) << 8)

#define MASK_NORTH_EAST(pos_mask)		((pos_mask) >> 9)
#define MASK_NORTH_WEST(pos_mask)		((pos_mask) >> 7)
#define MASK_SOUTH_EAST(pos_mask)		((pos_mask) << 7)
#define MASK_SOUTH_WEST(pos_mask)		((pos_mask) << 9)

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
		return pos_mask;
	}
};

#define GEN_NEXT_POS_MASK_CHANGER(MY_DIRECTION, MASK_CONV) 	\
template<int DIRECTION>										\
class NextPosMaskChanger<MY_DIRECTION> {						\
public:														\
	inline mask_t operator ()(const mask_t& pos_mask) {		\
		return MASK_CONV(pos_mask);							\
	}														\
};															\

GEN_NEXT_POS_MASK_CHANGER(RIGHT, 		MASK_EAST);
GEN_NEXT_POS_MASK_CHANGER(RIGHT_DOWN, 	MASK_SOUTH_EAST);
GEN_NEXT_POS_MASK_CHANGER(DOWN, 		MASK_SOUTH);
GEN_NEXT_POS_MASK_CHANGER(LEFT_DOWN, 	MASK_SOUTH_WEST);
GEN_NEXT_POS_MASK_CHANGER(LEFT, 		MASK_WEST);
GEN_NEXT_POS_MASK_CHANGER(LEFT_UP, 		MASK_NORTH_WEST);
GEN_NEXT_POS_MASK_CHANGER(UP, 			MASK_NORTH);
GEN_NEXT_POS_MASK_CHANGER(RIGHT_UP, 	MASK_NORTH_EAST);

typedef NextPosMaskChanger<0> Right;
typedef NextPosMaskChanger<1> RightDown;
typedef NextPosMaskChanger<2> Down;
typedef NextPosMaskChanger<3> LeftDown;
typedef NextPosMaskChanger<4> Left;
typedef NextPosMaskChanger<5> LeftUp;
typedef NextPosMaskChanger<6> Up;
typedef NextPosMaskChanger<7> RightUp;


#endif /* BITBOARD_MASK_H_ */
