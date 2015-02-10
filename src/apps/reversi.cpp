/**
 * File: reversi.cpp
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-01-24 18:52:25.759602
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "engine/engine.h"

#define TOTAL_THREADS 1

void human_vs_human() {
	HumanPlayer black, white;
	Game game(black, white);
	game.start();
}

void book_vs_book() {
	OpeningBookPlayer black, white;
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
//	EasyAIPlayer black;
//	EasyAIPlayer white;

//	Look1AIPlayer black;
//	Look1AIPlayer white;

//	Look2AIPlayer black;
//	Look2AIPlayer white;

//	RandomAIPlayer black;
//	RandomAIPlayer white;

//	MonteCarloAIPlayer black;
//	MonteCarloAIPlayer white;

//	CachedMonteCarloAIPlayer black;
//	CachedMonteCarloAIPlayer white;

	LookNAIPlayer black;
    LookNAIPlayer white;

//	NeuralNetworkAIPlayer black;
//	NeuralNetworkAIPlayer white;

//	CompoundAIPlayer black;
//	CompoundAIPlayer white;

	uint total = 1 / TOTAL_THREADS;
//	cin>>total;
	log_warn("start "<<total<<" games ...");
	uint win[4] = { 0, 0, 0, 0 };
	int total_diff = 0;
	//比赛，多比几盘
	timer begin;
	for_n(i, total)
	{
//		if (i%10==0)
//			log_warn("progress: "<<i<<"/"<<total<<" in "<<i<<" games: black:white="<<win[BLACK]<<":"<<win[WHITE]
//				<<" draw="<<win[DRAW]<<" diff="<<total_diff<<" avg="<<float(total_diff)/(i+1)<<" time="
//				<<begin.elapsed()<<" speed="<< i/begin.elapsed());
		Game game(black, white);
		Score score = game.start();
		win[score.winner] += 1;
		total_diff += score.diff();
	}
	log_warn("in "<<total<<" games: black:white="<<win[BLACK]<<":"<<win[WHITE]
		<<" draw="<<win[DRAW]<<" diff="<<total_diff<<" avg="<<float(total_diff)/total
		<<" time="<<begin.elapsed()<<" speed="<< total/begin.elapsed());
}

class GameTestThread: public thread {
public:
	void run() {
		AI_vs_AI();
	}
};

class GameTest {
public:
	GameTest() {
//		 human_vs_human();
//		 book_vs_book();
//		 human_vs_AI();
		 AI_vs_AI();
//		 multithread_test();
	}

	void multithread_test() {
		list<uint> job_ids;
		for_n(i, TOTAL_THREADS)
			job_ids.push_back(i);
		parallel<GameTestThread>(job_ids, TOTAL_THREADS, LOG_LEVEL_INFO);
	}
};

int main () {

	delete new GameTest;

//	NeuralNetworkTest a;

	return 0;
}

