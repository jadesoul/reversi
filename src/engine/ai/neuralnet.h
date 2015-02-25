#ifndef NEURALNET_H_1422972604_36
#define NEURALNET_H_1422972604_36
/**
 * File: neuralnet.h
 * Description: 
 *
 * Copyright (c) 2010-2013 Jadesoul (Home Page: http://jadesoul.sinaapp.com) <shenglan.wsl@alibaba-inc.com>
 * 
 * Date: 2015-02-03 22:10:04.365291
 * Written In: Alibaba-inc, Hangzhou, China
 */

#include "book.h"
#include "engine/neuralnet.h"
#include "search.h"

//class NeuralNetwork;

class NeuralNetworkAIPlayer : public OpeningBookPlayer {
private:
	NeuralNetwork network;
	LookNAIPlayer lookn;

public:
	NeuralNetworkAIPlayer();
	virtual pos_t play(Board& board);
};


#endif /* NEURALNET_H_1422972604_36 */
