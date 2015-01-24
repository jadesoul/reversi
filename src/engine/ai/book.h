#ifndef BOOK_H_1422092958_28
#define BOOK_H_1422092958_28
/**
 * File: book.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 17:49:18.283196
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"
#include "engine/openings.h"

//使用开局库的AI
class OpenBookPlayer : public AIPlayer {
public:
	virtual pos_t play(Board& board);
};

#endif /* BOOK_H_1422092958_28 */
