#ifndef ENGINE_H_1420019316_93
#define ENGINE_H_1420019316_93
/**
 * File: engine.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2014-12-31 17:48:36.925048
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "board.h"
#include "player.h"
#include "game.h"
#include "ai.h"
#include "human.h"
#include "neuralnet.h"

bool check1(const char* s) {
	int n = strlen(s);
	if (n != 65)
		return false;
	for (int i = 0; i < 64; ++i) {
		if (!(s[i] == '0' || s[i] == '1' || s[i] == '2'))
			return false;
	}
	if (!(s[64] == '1' || s[64] == '2'))
		return false;
	return true;
}

const char* server(const char* s) {
	if (!check1(s))
		return "";

	if (openings==NULL) openings = new OpeningBook();

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

	Game game(black, white);

	string query = s;
	return game.deal(query).c_str();
}

#endif /* ENGINE_H_1420019316_93 */
