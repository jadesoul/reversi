#ifndef MONTECARLO_H_1421853275_23
#define MONTECARLO_H_1421853275_23
/**
 * File: montecarlo.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 23:14:35.231425
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"
#include "engine/game.h"
#include "engine/openings.h"
#include "book.h"
#include "random.h"
#include "random_with_ending_search.h"

//利用蒙特卡罗搜索算法找出赢棋概率最大的走法
//基本思想:
//	针对当前局面所有可能的下子位置，衍生出一个新的残局，并同时扮演黑白的角色随机下棋，直到终局
//	对每个残局进行一局棋的推演称作一个轮回，对每个下子位置，重复执行若干个轮回，统计输赢情况
//	计算出每个位置的输赢概率，选择胜率最高的位置落子
class MonteCarloAIPlayer : public OpeningBookPlayer {
public:
	virtual pos_t play(Board& board);
};

#endif /* MONTECARLO_H_1421853275_23 */
