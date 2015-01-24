#ifndef HUMAN_H_1421852298_01
#define HUMAN_H_1421852298_01
/**
 * File: human.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-21 22:58:18.006800
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "player.h"

class HumanPlayer : public Player {
public:
	uchar play(Board& b);
};

#endif /* HUMAN_H_1421852298_01 */
