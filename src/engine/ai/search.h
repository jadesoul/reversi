#ifndef SEARCH_H_1421892047_31
#define SEARCH_H_1421892047_31
/**
 * File: search.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 10:00:47.310427
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/player.h"
#include "engine/state.h"
#include "engine/game.h"
#include "engine/score.h"
#include "book.h"

//向后看 N 步棋的 AI
class LookNAIPlayer : public OpeningBookPlayer {
private:
	uint total_nodes_for_each_game;

public:
	virtual inline void reset() { total_nodes_for_each_game=0; }
	virtual pos_t play(Board& board);
};

#endif /* SEARCH_H_1421892047_31 */
