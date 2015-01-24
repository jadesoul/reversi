#ifndef CACHED_MONTECARLO_H_1421892098_84
#define CACHED_MONTECARLO_H_1421892098_84
/**
 * File: cached_montecarlo.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 10:01:38.836256
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"
#include "random.h"
#include "engine/game.h"
#include "montecarlo.h"

//利用开局库缓存 + 蒙特卡罗搜索算法
class CachedMonteCarloAIPlayer : public MonteCarloAIPlayer {
public:
	void generate_big_book();

	virtual pos_t play(Board& board);
};


#endif /* CACHED_MONTECARLO_H_1421892098_84 */
