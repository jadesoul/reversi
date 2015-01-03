#ifndef GAME_TEST_H_1372368585_78
#define GAME_TEST_H_1372368585_78
/**
 * File: game_test.h
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2013-06-28 05:29:45.779000
 * Written In: Peking University, beijing, China
 */

#include "player.h"
#include "game.h"
#include "ai.h"

void human_vs_human() {
	HumanPlayer black, white;
	Game game(black, white);
	game.start();
}

void human_vs_AI() {
	EasyAIPlayer black;
	HumanPlayer white;
	Game game(black, white);
	game.start();
}

void AI_vs_AI() {
	// EasyAIPlayer black;
	// EasyAIPlayer white;

	// Look1AIPlayer black;
	// Look1AIPlayer white;

	// Look2AIPlayer black;
	// Look2AIPlayer white;

	RandomAIPlayer black;
	// RandomAIPlayer white;

	// MonteCarloAIPlayer black;
	// MonteCarloAIPlayer white;

	// LookNAIPlayer black;
	LookNAIPlayer white;

	uint total=1000;
	log_warn("start "<<total<<" games ...");
	uint win[3]={0, 0, 0};
	int total_diff=0;
	//比赛，多比几盘
	timer begin;
	for_n(i, total) {
		if (i%1==0)
			log_warn("progress: "<<i<<"/"<<total<<" in "<<i<<" games: black:white="<<win[BLACK]<<":"<<win[WHITE]
				<<" draw="<<win[DRAW]<<" diff="<<total_diff<<" avg="<<float(total_diff)/(i+1)<<" time="<<begin.elapsed()<<" speed="<< i/begin.elapsed());
		Game game(black, white);
		Score score=game.start();
		win[score.winner]+=1;
		total_diff+=score.diff();
	}
	log_warn("in "<<total<<" games: black:white="<<win[BLACK]<<":"<<win[WHITE]
		<<" draw="<<win[DRAW]<<" diff="<<total_diff<<" avg="<<float(total_diff)/total<<" time="<<begin.elapsed()<<" speed="<< total/begin.elapsed());
}

class GameTestThread : public thread {
	void run() {
		AI_vs_AI();
	}
};

class GameTest {
public:
	GameTest() {
		// human_vs_human();
		// human_vs_AI();
		AI_vs_AI();
		// multithread_test();
	}

	void multithread_test() {
		list<uint> job_ids;
		for_n(i, 8) job_ids.push_back(i);
		parallel<GameTestThread>(job_ids, 8, LOG_LEVEL_INFO);
	}
};

#endif /* GAME_TEST_H_1372368585_78 */
