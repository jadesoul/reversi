#ifndef BITBOARD_BITOP_H_
#define BITBOARD_BITOP_H_
/*
 * File: bitop.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月28日
 * Written In: Alibaba-inc, Hangzhou, China
 */

//64位bits 操作

#define ONE								static_cast<uint64_t>(0x01)//仅最低位是1
#define ALLONE							static_cast<uint64_t>(-1)//所有的位全是1

//#define BIT_EXIST(bits, offset)		( (bits >> offset) & 0x01 )
#define BIT_EXIST(bits, offset)			( bits & (ONE << offset) )
#define BIT_NOT_EXIST(bits, offset)		( ! BIT_EXIST(bits, offset) )

#define SET_BIT(bits, offset)			( bits |= (ONE << offset) )
#define CLEAR_BIT(bits, offset)			( bits &= (ALLONE ^ (ONE << offset) ) )
#define FLIP_BIT(bits, offset)			( bits ^= (ONE << offset) )
#define REVERSE_BITS(bits)				( bits ^= ALLONE )

#define IS_EMPTY(my, op, pos)			BIT_NOT_EXIST((my | op), pos)

#endif /* BITBOARD_BITOP_H_ */
