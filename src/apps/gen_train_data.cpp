/*
 * File: gen_train_data.cpp
 * Description:
 *
 * Copyright (c) 2010-2015 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 *
 * Author: jadesoul
 * Date: 2015年2月15日
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/engine.h"

int main() {
	RandomWithEndingSearchAIPlayer black;
	RandomWithEndingSearchAIPlayer white;

	uint total = 100;
	for_n(i, total)
	{
		if (i%10==0) log_warn("progress: "<<i<<"/"<<total);
		Game game(black, white);
		Score score = game.start();
		game.get_board().dump_history(cout);
	}
}
