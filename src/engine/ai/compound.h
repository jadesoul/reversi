#ifndef COMPOUND_H_1422100077_77
#define COMPOUND_H_1422100077_77
/**
 * File: compound.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 19:47:57.772798
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "book.h"
#include "look2.h"
#include "search.h"

//合成各种AI的特长
class CompoundAIPlayer : public OpeningBookPlayer {
private:
	Look2AIPlayer look2;
	LookNAIPlayer lookn;
public:
	virtual pos_t play(Board& board);
};


#endif /* COMPOUND_H_1422100077_77 */
