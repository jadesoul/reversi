#ifndef HASH_H_1421891912_21
#define HASH_H_1421891912_21
/**
 * File: hash.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:58:32.207848
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "common.h"

typedef uint64_t hash_t;

//地图上每个位置都可以取5种状态，对应一个唯一的hash值
//最后追加一个虚拟位置代表TURN，即当前轮谁下，可以取值BLACK和WHITE
//当每个位置取一个确定的值时，对应的局面的hash就是这所有hash值的亦或值
extern hash_t board_hash_table[MAP_SIZE+1][5];

void init_board_hash_table();

#endif /* HASH_H_1421891912_21 */
