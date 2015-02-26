#ifndef BITBOARD_LOG2_H_
#define BITBOARD_LOG2_H_
/*
 * File: log2.h
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

//返回64bit二进制串从左往右数的第一个1的下标,例外：x=0 返回63
int log2_u64_r(uint64_t x) {
	int n = 63;
    if (x & 0xffffffff00000000) { n -= 32;  x >>= 32;   }
    if (x & 0xffff0000) 		{ n -= 16;  x >>= 16;   }
    if (x & 0xff00)     		{ n -= 8;   x >>= 8;    }
    if (x & 0xf0)       		{ n -= 4;   x >>= 4;    }
    if (x & 0xc)        		{ n -= 2;   x >>= 2;    }
    if (x & 0x2)        		{ --n;                  }
    return n;
}

//返回32bit二进制串从左往右数的第一个1的下标,例外：x=0 返回31
int log2_u32_r(uint32_t x) {
	int n = 31;
    if (x & 0xffff0000) { n -= 16;  x >>= 16;   }
    if (x & 0xff00)     { n -= 8;   x >>= 8;    }
    if (x & 0xf0)       { n -= 4;   x >>= 4;    }
    if (x & 0xc)        { n -= 2;   x >>= 2;    }
    if (x & 0x2)        { --n;                  }
    return n;
}

//返回64bit二进制串从右往左数的最后一个1的下标(下标也是从右往左数),例外：x=0 返回0
int log2_u64(uint64_t x) {
	int n = 0;
    if (x & 0xffffffff00000000) { n += 32;  x >>= 32;   }
    if (x & 0xffff0000) 		{ n += 16;  x >>= 16;   }
    if (x & 0xff00)     		{ n += 8;   x >>= 8;    }
    if (x & 0xf0)       		{ n += 4;   x >>= 4;    }
    if (x & 0xc)        		{ n += 2;   x >>= 2;    }
    if (x & 0x2)        		{ ++n;                  }
    return n;
}

//返回32bit二进制串从右往左数的最后一个1的下标(下标也是从右往左数),例外：x=0 返回0
int log2_u32(uint32_t x) {
	int n = 0;
    if (x & 0xffff0000) { n += 16;  x >>= 16;   }
    if (x & 0xff00)     { n += 8;   x >>= 8;    }
    if (x & 0xf0)       { n += 4;   x >>= 4;    }
    if (x & 0xc)        { n += 2;   x >>= 2;    }
    if (x & 0x2)        { ++n;                  }
    return n;
}

//查此表可以返回一个8bits的log2对数+1后的值
const static int fast_log2_u8_table[256]={
	0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
	6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
	8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
};

//快速返回64bit二进制串从右往左数的最后一个1的下标(下标也是从右往左数),例外：x=0 返回-1
int fast_log2_u64(uint64_t x) {
	int n = -1;
    if (x & 0xffffffff00000000) { n += 32;  x >>= 32;   }
    if (x & 0xffff0000) 		{ n += 16;  x >>= 16;   }
    if (x & 0xff00)     		{ n += 8;   x >>= 8;    }
    return n + fast_log2_u8_table[x];
}

//快速返回32bit二进制串从右往左数的最后一个1的下标(下标也是从右往左数),例外：x=0 返回-1
int fast_log2_u32(uint32_t x) {
	int n = -1;
    if (x & 0xffff0000) { n += 16;  x >>= 16;   }
    if (x & 0xff00)     { n += 8;   x >>= 8;    }
    return n + fast_log2_u8_table[x];
}

//来自Quake III 引擎中的神奇代码
static const int debruijn[32] = {//经过一个特殊处理后的高5位概率分布
     0,  1, 28,  2, 29, 14, 24,  3, 30, 22, 20, 15, 25, 17,  4,  8,
    31, 27, 13, 23, 21, 19, 16,  7, 26, 12, 18,  6, 11,  5, 10,  9
};
#define LOG2_QUAKE_III(x) (debruijn[((uint32_t)(((x) & -(x)) * 0x077CB531U)) >> 27])
// (x) & (-x) 的意思是取出 x 末尾的 0 和从右往左第一次出现的 1

//快速计算的宏
#define LOG2(x) ( \
					((x & 0xffffffff00000000)!=0)*32 \
					+ (((x>>32) & 0xffff0000)!=0)*16 \
					+ (((x>>48) & 0xff00)!=0)*8 \
					+ (fast_log2_u8_table[x>>56]) \
					-1 \
				)

#define LOG2_BYTE(x)	(fast_log2_u8_table[x]-1)

#endif /* BITBOARD_LOG2_H_ */
