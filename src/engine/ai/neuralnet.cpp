/**
 * File: neuralnet.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-03 22:10:08.146149
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "neuralnet.h"
#include "random.h"

NeuralNetworkAIPlayer::NeuralNetworkAIPlayer() {

	log_warn("start generating big book, size="<<openings->book.size());
	RandomAIPlayer player;//用于推演时随机下棋
	//利用蒙特卡罗思想，开始随机下棋若干次
	uint total=10000;
	for_n(i, total) {
		if (i%1000==0) log_warn("generated: "<<i<<"/"<<total<<"="<<(float(i)/total));
		Game game(player, player);//举办一场比赛
		game.start_expand_opening(openings->book);
	}
	log_warn("generated big book, size="<<openings->book.size());

	network.train();
}

pos_t NeuralNetworkAIPlayer::play(Board& board) {
	color self=board.turn();

//		move_t opening_move=openings->lookup(b);
//		if (opening_move!=PASS) {
//			assert(b.is_active(opening_move));
//			b.play(opening_move);
//			return opening_move;
//		}

	pos_t best_move=network.predict(board);

	assert(best_move!=PASS);
	log_info(COLOR(self)<<" NeuralNetworkAIPlayer, play at "<<Move(self, best_move));
	board.play(best_move);
	return best_move;
}
