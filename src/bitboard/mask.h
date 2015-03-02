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
#define MASK_EAST(pos_mask)				((pos_mask) << 1)//注意。。。
#define MASK_WEST(pos_mask)				((pos_mask) >> 1)//注意，下一排的第一个会循环到上一排的最后一个
#define MASK_NORTH(pos_mask)			((pos_mask) >> 8)
#define MASK_SOUTH(pos_mask)			((pos_mask) << 8)

#define MASK_NORTH_EAST(pos_mask)		((pos_mask) >> 7)
#define MASK_NORTH_WEST(pos_mask)		((pos_mask) >> 9)
#define MASK_SOUTH_EAST(pos_mask)		((pos_mask) << 9)
#define MASK_SOUTH_WEST(pos_mask)		((pos_mask) << 7)

class Mask {
public:
	mask_t mask;

	Mask(mask_t mask): mask(mask) {}

	friend inline ostream& operator<<(ostream& o, const Mask& m) { m.dump(o); return o; }

	void dump(ostream& o) const {
		mask_t m=mask;
		for_n(i, 64) {
			if (i%8==0) o<< "\n";
			o<< ((m & 0x01) ? '1' : '.');
			o<<' ';
			m >>= 1;
		}
		o<< "\n\n";
	}
};

#endif /* BITBOARD_MASK_H_ */
