/**
 * File: random.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 16:41:47.542530
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "random.h"

uint Random::instance_cnt = 0;

Random::Random() {
	if (instance_cnt > 0)
		return; //防止多次初始化
	srand((int) time(NULL)); //初始化随机数种子
	instance_cnt += 1;
}

int Random::randint(int start, int end) const {
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
