#ifndef EASY_H_1421891030_52
#define EASY_H_1421891030_52
/**
 * File: easy.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-22 09:43:50.520303
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "book.h"

//最简单的AI，选择第一个可下子的位置下子
class EasyAIPlayer : public OpeningBookPlayer {
public:
	virtual pos_t play(Board& board);
};

#endif /* EASY_H_1421891030_52 */
