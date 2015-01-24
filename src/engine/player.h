#ifndef PLAYER_H_1371915057_31
#define PLAYER_H_1371915057_31
/**
 * File: player.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-22 23:30:57.305000
 * Written In: Peking University, beijing, China
 */

#include "board.h"
#include "openings.h"

//所有玩家的抽象基类
class Player {
public:
	//下子的抽象接口，返回一个字节共8bit，(前4bit, 后4bit)代表下子位置(x, y)
	//调用此函数前，需确保有子可下
	virtual uchar play(Board& b)=0;
	virtual void reset() {}
	virtual ~Player() {}
};

//所有AI的基类
//class AIPlayer : public Player {};

//optimized here:
typedef Player AIPlayer;

#endif /* PLAYER_H_1371915057_31 */
