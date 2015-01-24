#ifndef LOOK1_H_1421891961_96
#define LOOK1_H_1421891961_96
/**
 * File: look1.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:59:21.960684
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "book.h"

//向后看一步棋的AI
class Look1AIPlayer : public OpeningBookPlayer {
public:
	virtual pos_t play(Board& board);
};

#endif /* LOOK1_H_1421891961_96 */
