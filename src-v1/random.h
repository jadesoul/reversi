#ifndef RANDOM_H_1356527036_22
#define RANDOM_H_1356527036_22
/**
 * File: random.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2012-12-26 21:03:56.225000
 * Written In: Peking University, beijing, China
 */

#include "bootstrap.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

class Random { //最好只有一个实例（用单例模式 ? 太麻烦，算了）
private:
	static uint instance_cnt;
public:
	Random() {
		if (instance_cnt > 0)
			return; //防止多次初始化
		srand((int) time(NULL)); //初始化随机数种子
		instance_cnt += 1;
	}

	//返回区间[0, size)之间的随机数
	uint randindex(uint size) const {
		assert(size<=RAND_MAX); //支持的最大上界
		return rand() % size;
	}

	//返回区间[0, max]之间的随机数
	uint randuint(uint max) const {
		assert(max<=RAND_MAX); //支持的最大上界
		return rand() % (max + 1);
	}

	//返回区间[start, end]或[end, start]之间的随机数
	int randint(int start, int end) const {
		if (start == end)
			return start;
		if (start < end) {
			uint len = end - start;
			return start + randuint(len);
		} else {
			uint len = start - end;
			return end + randuint(len);
		}
	}
};

uint Random::instance_cnt = 0;


#endif /* RANDOM_H_1356527036_22 */

