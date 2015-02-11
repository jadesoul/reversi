#ifndef RANDOM_WITH_ENDING_SEARCH_H_1423653313_3
#define RANDOM_WITH_ENDING_SEARCH_H_1423653313_3
/**
 * File: random_with_ending_search.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-11 19:15:13.296783
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "random.h"
#include "book.h"
#include "search.h"

//也是很简单的AI，随机选择一个可下子的位置下子
//棋力灰常弱，可以作为Baseline
class RandomWithEndingSearchAIPlayer : public LookNAIPlayer {
	RandomAIPlayer random;
public:
	virtual pos_t play(Board& board);
};

#endif /* RANDOM_WITH_ENDING_SEARCH_H_1423653313_3 */
