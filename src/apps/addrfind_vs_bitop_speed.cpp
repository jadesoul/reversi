/*
 * File: addrfind_vs_bitop_speed.cpp
 * Description: 测试寻址操作与位运算操作的速度，总体结论：位运算会快3-4个数量级
 * 				写操作比取操作慢1个数量级
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月25日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/common.h"

//typedef uint32_t ulong;
typedef uint64_t ulong;

#define TOTAL	100000000

double find(ulong& s) {
	uint d[64];
	uint o[64];
	for_n(i, 64) { d[i]=rand(); o[i]= 0x01 << i; }

	ulong a;
	timer now;
	for_n(j, TOTAL) for_n(i, 64) {
		a= d[i];
		d[i]=a+i;
		s+=a;
	}
	return now.elapsed();
}

double bitop(ulong& s) {

	uint d[64];
	uint o[64];
	for_n(i, 64) { d[i]=rand(); o[i]= 0x01 << i; }

	ulong a;
	timer now;
	for_n(j, TOTAL) for_n(i, 64) {

//		a= (a & (0x01 << i)) ? i : -i;
//		a= (a & o[i]) ? i : -i ;
		a= a & (0x01 << i);
//		a = a & o[i];
		s+=a;
	}
	return now.elapsed();
}

int main() {
	srand(time(NULL));

	ulong s;
	double gap1=find(s);
	cout<<"find:\t"<<gap1<<endl;
	double gap2=bitop(s);
	cout<<"bitop:\t"<<gap2<<endl;
	return 0;
}

