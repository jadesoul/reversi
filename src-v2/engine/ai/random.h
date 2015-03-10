#ifndef RANDOM_H_1421891813_43
#define RANDOM_H_1421891813_43
/**
 * File: random.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:56:53.425544
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"

//也是很简单的AI，随机选择一个可下子的位置下子
//棋力灰常弱，可以作为Baseline
class RandomAIPlayer : public AIPlayer {
	Random random;
public:
	virtual pos_t play(Board& board);
};


#endif /* RANDOM_H_1421891813_43 */
