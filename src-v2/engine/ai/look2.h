#ifndef LOOK2_H_1421891987_3
#define LOOK2_H_1421891987_3
/**
 * File: look2.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:59:47.304504
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "book.h"

//向后看2步棋的AI
class Look2AIPlayer : public OpeningBookPlayer {
public:
	virtual pos_t play(Board& board);
};


#endif /* LOOK2_H_1421891987_3 */
